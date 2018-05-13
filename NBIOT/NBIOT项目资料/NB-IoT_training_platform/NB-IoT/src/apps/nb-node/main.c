#include "board.h"

#define IWDG_REFRESH_DUTYCYCLE 3000

/*!
 * Defines the application data size
 */
#define APP_DATA_SIZE      22

/*!
 * Defines get net status retry numbers
 */
#define GET_NETSTAT_RETRY_MAX 24

/*!
 * Defines max lost numbers for udp uplink
 */
#define PACKET_LOST_MAX  15

/*!
 * Specify node communication status
 */
typedef struct{
    uint32_t node_id;
    uint16_t up_cnt;
    uint16_t down_cnt;
    uint16_t up_lost;
    uint16_t send_err_cnt; // specify udp send error
    uint16_t lost_cnt;     // when too many packet lost,this variable increse
}statistic_t;

typedef enum{
    UDP_UPLINK,
    UDP_DOWNLINK,
    IDLE
}run_mode_t;

/*!
 * Defines the application data
 */
static uint8_t app_data[APP_DATA_SIZE];
static uint8_t rec_data[APP_DATA_SIZE];
static uint8_t mac[] = REMOTE_MAC;

static timer_t iwdg_refresh_timer;
static timer_t appdata_send_timer;

static int udpsocket =0;
/*!
 * Specify whether appdata ready to send
 */
static bool is_appdata_prepare = true;

run_mode_t run_mode;
statistic_t node_stat;
/*!
 * \brief   Packet assembly
 */
static void prepare_data()
{
    int32_t latitude, longitude = 0;
    int16_t altitudeGps = 0xFFFF;
    uint16_t humi = 0, temp = 0;
    uint8_t node_id[NODE_ID_SIZE] = NODE_ID;
    GpsGetLatestGpsPositionBinary(&latitude, &longitude);
    altitudeGps = GpsGetLatestGpsAltitude();                            // in m
    SHT2xGetTempHumi(&temp, &humi);

    app_data[0] = node_id[0];
    app_data[1] = node_id[1];
    app_data[2] = node_id[2];
    app_data[3] = node_id[3];
    memcpy(&app_data[4], mac, 6);
    app_data[10] = (uint8_t)((latitude >> 16) & 0xFF);
    app_data[11] = (uint8_t)((latitude >> 8) & 0xFF);
    app_data[12] = (uint8_t)(latitude & 0xFF);
    app_data[13] = (uint8_t)((longitude >> 16) & 0xFF);
    app_data[14] = (uint8_t)((longitude >> 8) & 0xFF);
    app_data[15] = (uint8_t)(longitude & 0xFF);
    app_data[16] = (uint8_t)((altitudeGps >> 8) & 0xFF);
    app_data[17] = (uint8_t)(altitudeGps & 0xFF);
    app_data[18] = (uint8_t)((temp >> 8) & 0xFF);
    app_data[19] = (uint8_t)(temp & 0xFF);
    app_data[20] = (uint8_t)((humi >> 8) & 0xFF);
    app_data[21] = (uint8_t)(humi & 0xFF);
    clear_buf();
}

static uint8_t parse_downlink(uint8_t *data, uint16_t size)
{
    return 0;
}

static void iwdg_refresh_event()
{
    //log("refresh.\n");
    iwdog_refresh();
}

static void appdata_send_event()
{
    is_appdata_prepare = true;
    timer_stop(&appdata_send_timer);
}

void net_attach()
{
    uint8_t try_cnt = 0;
    int ret = 0;

    while(1)
    {
        // AT+NRB
        neul_bc95_reboot();

        // AT+CGATT?
        do{
            log("get net status:%d\n", try_cnt);
            ret = neul_bc95_get_netstat();
            if(!ret)
            {
                log("connected.\n");
                return ;
            }
            neul_bc95_sleep(5000);
        } while(ret < 0 && ++try_cnt < GET_NETSTAT_RETRY_MAX);
        try_cnt = 0;
        log("network disconnect.reboot!\n");
    }
}

bool udp_uplink()
{
    int ret = 0;
    uint8_t i;

    if(is_appdata_prepare == false)
        return false;
    prepare_data();
    log("udp send...\n");
    for(i = 0; i < APP_DATA_SIZE; i++)
    {
        log("data[%i]:%x\n", i, app_data[i]);
    }
    ret = neul_bc95_udp_send(udpsocket, (const char *)app_data, APP_DATA_SIZE);
    if(ret == 0)
    {
        log("udp send ok.\n");
        is_appdata_prepare = false;
        return true;
    }
    else
    {
        log("udp send fail.\n");

        // reboot and attch net
        net_attach();
        node_stat.send_err_cnt++;

        // udp socket create
        log("create udp socket...\n");
        if((ret = neul_bc95_create_udpsocket(8091)) >= 0)
        {
            udpsocket = ret;
            log("ok.\n");
        }
        return false;
    }
}

bool udp_downlink()
{
    uint8_t rec_cnt = 0;
    uint8_t i;
    int ret = 0;

    while(rec_cnt++ < 5)
    {
        ret = neul_bc95_udp_read(udpsocket, (char *)rec_data, APP_DATA_SIZE, 0);
        log("downlink: %d\n", ret);
        // receive packet
        if(ret > 0)
        {
            // print down link data
            for(i = 0; i < ret; i++)
            {
               log("data[%i]:%x\n", i, rec_data[i]);
            }

            // data parse
            parse_downlink(rec_data, APP_DATA_SIZE);
            return true;
        }
        neul_bc95_sleep(5000);
    }
    return false;
}

int main(){
    int ret = 0;
    uint8_t node_id[NODE_ID_SIZE] = NODE_ID;

    BoardInitMcu();
    BoardInitPeriph();

    // watchdog init
    iwdog_init();
    timer_create(&iwdg_refresh_timer, IWDG_REFRESH_DUTYCYCLE, OPT_TMR_PERIODIC, iwdg_refresh_event);
    timer_start(&iwdg_refresh_timer);

    // app data send timer init
    timer_create(&appdata_send_timer, APP_TX_DUTYCYCLE, OPT_TMR_PERIODIC, appdata_send_event);

    // nb init
    while((ret = neul_bc95_hw_init()) != 0)
    {
        log("init fail,reboot.\n");
    }
    log("init success.\n");

    node_stat.node_id = node_id[0]<<24 | node_id[1]<<16 | node_id[2]<<8 | node_id[3];

    // reboot and get netstat
    net_attach();
    log_info("network attached.\n");

    // udp socket create
    log("create udp socket...\n");
    if((ret = neul_bc95_create_udpsocket(8091)) >= 0)
    {
        udpsocket = ret;
        log("ok.\n");
    }

    run_mode = UDP_UPLINK;
    // udp client
    while(1)
    {
        switch(run_mode)
        {
            case UDP_UPLINK:
            {
                log_info("id: %x, upcnt: %d, downcnt: %d, send_err: %d, lost_cnt: %d\n", node_stat.node_id, node_stat.up_cnt, node_stat.down_cnt, \
                node_stat.send_err_cnt, node_stat.lost_cnt);
                if(udp_uplink() == true)
                {
                    run_mode = UDP_DOWNLINK;
                    node_stat.up_cnt++;
                    node_stat.up_lost++;
                    log("uplink lost:%d\n", node_stat.up_lost);
                }
                else
                {
                    run_mode = IDLE;
                }
                timer_start(&appdata_send_timer);
                break;
            }
            case UDP_DOWNLINK:
            {
                if(udp_downlink() == true)
                {
                    node_stat.down_cnt++;
                    node_stat.up_lost = 0;
                }
                if(node_stat.up_lost >= PACKET_LOST_MAX)
                {
                    log("too many packet lost.\n");
                    node_stat.up_lost = 0;
                    node_stat.lost_cnt++;
                    // reboot and attch net
                    net_attach();

                    // udp socket create
                    log("create udp socket...\n");
                    if((ret = neul_bc95_create_udpsocket(8091)) >= 0)
                    {
                        udpsocket = ret;
                        log("ok.\n");
                    }
                }
                log("enter idle mode.\n");
                run_mode = IDLE;
                break;
            }
            case IDLE:
                if(is_appdata_prepare == true)
                    run_mode = UDP_UPLINK;
                break;
        }
    }

}
