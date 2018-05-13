#include "nb.h"

//Defines uart buffer size
#define BUFFER_TX_SIZE                                128
#define BUFFER_RX_SIZE                                128

#define COMFIRM_DELAY         1000
#define COMMAND_SEND_RETRY    5

static uint8_t NB_Tx_buffer[BUFFER_TX_SIZE];
static uint8_t NB_Rx_buffer[BUFFER_RX_SIZE];

//Defines the use of 'AT' command
static char Command_type[][8] = {
"NBAND",
"NRB",
"NCONFIG",
"CFUN",
"CSQ",
"CGATT",
"CEREG",
"CSCON",
"NSOCR",
"NSOST",
"NSONMI",
"AT"
};

//Define strings type command
static char Command_strings[COMMAND_STRING_SIZE] = "AT";

static uint8_t String_Size = 0;

//Storge response string that receive form nb module
static char Response_string[RESPONSE_STRING_SIZE];

static uint8_t Response_string_size = 0;

//Storge parsed response data
static tResponse Response_data;

static uint8_t Has_new_response =false;

static bool Is_socket_creat = false;

static bool Is_nb_ready = false;

static bool confirm(tConfirm type);

static void uart_send(){
    UartPutBuffer(&Uart1, (uint8_t *)Command_strings, String_Size);
    log(Command_strings);
}

static void command_send(tCommand command, tType type, const char * para){
    Command_strings[0] = '\0';

    //test command 
    if(strcmp(Command_type[command], "AT") == 0){
        strcat(Command_strings, "AT");
    }
    else{
        strcat(Command_strings, "AT+");
        strcat(Command_strings, Command_type[command]);
    }
    if(type == T_READ){
        strcat(Command_strings, "?");  
    }
    else if(type == T_SET){
        if(para != NULL){
            strcat(Command_strings, para); 
        }
    }
    strcat(Command_strings, "\r\n");
    String_Size = strlen(Command_strings);
    uart_send();
}

static void delay(uint32_t Delay){
    HAL_Delay(Delay);
}

static bool reponse_format(uint8_t *string, uint16_t size){
    uint8_t fieldSize;
    uint8_t i = 3;
    uint8_t j = 0;

    log("Start formating...\n");
    if(string[0] != '\r' || string[1] != '\n' || string[2] != '+'){
        return false;
    }
    
    //get command type (for example : NBAND)
    fieldSize = 0;
    while(string[i + fieldSize++] != ':')
    {
        if(fieldSize > 10)
        {
            return false;
        }
    }
    for(j = 0; j < fieldSize-1; j++, i++)
    {
        Response_data.cmd[j] = string[i];
        
    }
    Response_data.cmd[j++] = '\0';
    i++;
    log("cmd: %s\n", Response_data.cmd);
    
    //get first parameter
    fieldSize = 0;
    while(string[i + fieldSize] != ',')
    {
        if(fieldSize > 15)
        {
            return false;
        }
        //if that's the end patameter 
        else if(string[i + fieldSize] == '\r'){
            for(j = 0; j < fieldSize; j++, i++)
            {
                Response_data.para1[j] = string[i];
            }
            Response_data.para1[j++] = '\0';
            i++;
            log("para1: %s\n", Response_data.para1);
            return true;
        }
        fieldSize++;
    }
    for(j = 0; j < fieldSize; j++, i++)
    {
        Response_data.para1[j] = string[i];
    }
    Response_data.para1[j++] = '\0';
    i++;
    log("para1: %s\n", Response_data.para1);
    
    //get second parameter
    fieldSize = 0;
    while(string[i + fieldSize] != ',')
    {
        if(fieldSize > 15)
        {
            return false;
        }
        //if that's the end patameter 
        else if(string[i + fieldSize] == '\r'){
            for(j = 0; j < fieldSize; j++, i++)
            {
                Response_data.para2[j] = string[i];
            }
            Response_data.para2[j++] = '\0';
            i++;
            log("para2: %s\n", Response_data.para2);
            return true;
        }
        fieldSize++;
    }
    for(j = 0; j < fieldSize; j++, i++)
    {
        Response_data.para2[j] = string[i];
    }
    Response_data.para2[j] = '\0';
    i++;
    log("para2: %s\n", Response_data.para2);
    return true;
}

static void nb_irq_notify(UartNotifyId_t id){
    uint8_t data;
    if(id == UART_NOTIFY_RX){
        if(UartGetChar(&Uart1, &data) == 0){
            //UartPutChar(&Uart1, data);
            if(Has_new_response == false){           
                Response_string[Response_string_size++] = (uint8_t)data;
                if(data == '\n' && \
                    Response_string[Response_string_size-3] == 'K' && \
                    Response_string[Response_string_size-4] == 'O'
                    ){
                    log("new response:%s", Response_string);
                    Response_string[Response_string_size] = '\0';
                    //reponse_format((uint8_t*)Response_string, Response_string_size);
                    Has_new_response = true;
                }
            }
        }
    }
}

static bool confirm(tConfirm type){
    bool temp;
    uint32_t tickstart = 0U;
    
    tickstart = HAL_GetTick();
    Has_new_response = false;
    Response_string_size = 0;
    while(Has_new_response != true){
        if((HAL_GetTick() - tickstart) > COMFIRM_DELAY)
            return false;
    }
    
    if(type == CONFIRM_OK_ONLY){

        return true;
    }
    else if(type == CONFIRM_PARA){
        reponse_format((uint8_t*)Response_string, Response_string_size);
        
        //confirm response data
        if(strcmp((const char*)Response_data.cmd, (const char*)Command_type[CSQ]) == 0){
            log("Confirm CSQ...\n");
            log("para:%d\n", str2int((const char *)Response_data.para1));
            if(str2int((const char *)Response_data.para1) == 99)
                temp = false;
            else
                temp = true;  
        }
        else if(strcmp((const char*)Response_data.cmd, (const char*)Command_type[NBAND]) == 0){
            log("Confirm NBAND...\n");
            log("para:%d\n", str2int((const char *)Response_data.para1));
            if(str2int((const char *)Response_data.para1) == 5)
                temp = true;
            else
                temp = false;  
        }
        else if(strcmp((const char*)Response_data.cmd, (const char*)Command_type[CGATT]) == 0){
            log("Confirm CGATT...\n");
            log("para:%d\n", str2int((const char *)Response_data.para1));
            if(str2int((const char *)Response_data.para1) == 1)
                temp = true;
            else
                temp = false;  
        }    
        else if(strcmp((const char*)Response_data.cmd, (const char*)Command_type[CEREG]) == 0){
            log("Confirm CEREG...\n");
            log("para:%d\n", str2int((const char *)Response_data.para2));
            if(str2int((const char *)Response_data.para2) == 1)
                temp = true;
            else
                temp = false;
        }
        else{
            temp = false;
        }
        return temp;
    }
    return false;
}

bool nb_start(){
    static uint8_t retry = 0;

    //AT
    do{
        command_send(AT, T_SET, NULL);
        if(retry++ > COMMAND_SEND_RETRY)
            return false;
    }while(confirm(CONFIRM_OK_ONLY) == false);
    retry = 0;

    //AT+NBAND=5
    command_send(NBAND, T_SET, "=5");
    if(confirm(CONFIRM_OK_ONLY) == false)
        return false;
    delay(500);
    
    //AT+NCONFIG=AUTOCONECT,TRUE
    command_send(NCONFIG, T_SET, "=AUTOCONNECT,TRUE");
    if(confirm(CONFIRM_OK_ONLY) == false)
        return false;
    delay(500);

    //AT+NCONFIG=CR_0354_0338_SCRAMBLING,TRUE
    command_send(NCONFIG, T_SET, "=CR_0354_0338_SCRAMBLING,TRUE");
    if(confirm(CONFIRM_OK_ONLY) == false)
        return false;
    delay(500);
    
    //AT+NCONFIG=CR_0859_SI_AVOID,TRUE
    command_send(NCONFIG, T_SET, "=CR_0859_SI_AVOID,TRUE");
    if(confirm(CONFIRM_OK_ONLY) == false)
        return false;
    delay(500);
    
    //AT+CFUN=1
    command_send(CFUN, T_SET, "=1");
    if(confirm(CONFIRM_OK_ONLY) == false)
        return false;  
    delay(500);

    //AT+NRB
    command_send(NRB, T_SET, NULL);
    delay(4000);    
    if(confirm(CONFIRM_OK_ONLY) == false)
        return false;  
    delay(5000);

    //AT+CSQ
    do{
        delay(5000);
        command_send(CSQ, T_SET, NULL);
        if(retry++ > COMMAND_SEND_RETRY)
            return false;
    }while(confirm(CONFIRM_PARA) == false);
    retry = 0;
    
    //AT+CGATT?    
    do{ 
        delay(5000);
        command_send(CGATT, T_READ, NULL);
        if(retry++ > COMMAND_SEND_RETRY)
            return false;
    }while(confirm(CONFIRM_PARA) == false);
    retry = 0;
    
    //AT+CEREG?
    do{ 
        command_send(CEREG, T_READ, NULL);
        if(retry++ > COMMAND_SEND_RETRY)
            return false;
    }
    while(confirm(CONFIRM_PARA) == false);
    retry = 0;   
    Is_nb_ready = true;
    return true;
}

bool nb_init(){

    /*config uart1 to transfer at command*/
    FifoInit(&Uart1.FifoTx, NB_Tx_buffer, BUFFER_TX_SIZE);
    FifoInit(&Uart1.FifoRx, NB_Rx_buffer, BUFFER_RX_SIZE);
    Uart1.IrqNotify = nb_irq_notify;    
    UartInit(&Uart1, UART_1, UART_TX, UART_RX);
    UartConfig(&Uart1, RX_TX, 9600, UART_8_BIT, UART_1_STOP_BIT, NO_PARITY, NO_FLOW_CTRL);  

    if(nb_start() == true)
        return true;
    else
        return false;
}

bool udp_creat_socket(){
    uint8_t retry = 0;
    
    if(Is_socket_creat == false && Is_nb_ready== true)
    {     
        do{ 
            command_send(NSOCR, T_SET, "=DGRAM,17,8091,1");
            if(retry++ > COMMAND_SEND_RETRY)
                return false;
        }
        while(confirm(CONFIRM_OK_ONLY) == false);
        Is_socket_creat = true;
        return true;
    }
    return false;
}

bool udp_send(uint8_t *data, const char *ip, const char*port){
    int ilen;
    char slen[3];
    char packet[PAYLOAD_SIZE + PACKET_HEAD_SIZE] = {'\0'};
    
    if(Is_socket_creat == false || data == NULL || ip == NULL || port == NULL)
        return false;
    
    log("ip: %s \n", ip);
    log("port: %s \n", port);

    if((ilen = strlen((const char*)data)/2) > PAYLOAD_SIZE)
        return false;

    slen[0] = ilen/10 + '0';
    slen[1] = ilen%10 + '0';
    slen[2] = '\0';

    //assemble udp packet
    strcat(packet, "=0,");
    strcat(packet, ip);
    strcat(packet, ",");
    strcat(packet, port);
    strcat(packet, ",");
    strcat(packet, slen);
    strcat(packet, ",");
    strcat(packet, (const char *)data);

    command_send(NSOST, T_SET, packet);
    if(confirm(CONFIRM_OK_ONLY) == false)
        return false;
    
    Is_socket_creat = true;
    return true;
}

