
$(document).ready(function () {
    //日历
    $('#datepicker1').datepicker();
    $('#datepicker2').datepicker();
    setDate();
    $("#go").mousedown(function () {
        $("#go").css({
            "background-color": "#C02020"
        })
    })
    $("#go").mouseup(function () {
        $("#go").css({
            "background-color": "#00CFFF"
        })
    })

    var bannerSlider = new Slider($('#banner_tabs'), {
        time: 5000,
        delay: 400,
        event: 'hover',
        auto: true,
        mode: 'fade',
        controller: $('#bannerCtrl'),
        activeControllerCls: 'active'
    });
    $('#banner_tabs .flex-prev').click(function () {
        bannerSlider.prev()
    });
    $('#banner_tabs .flex-next').click(function () {
        bannerSlider.next()
    });
    //提交
    $("#go").on("click", function () {
        datas = [];
        pages = 0;
        $("#go").addClass("gone");
        param = {
            sTime: getDate("#datepicker1"),
            "readerid": getID(),
            "datatype": "json"
        }
        load_Char_Grid(param, "go");
        //    loadChar(param);
    })
    //char grid slide
    $(".title_head").click(function (e) {
        if ($(e.target).siblings().attr("style") == undefined || $(e.target).siblings().attr("style").indexOf("display: none;") < 0) {
            $(e.target).siblings().slideUp()
        } else {
            $(e.target).siblings().slideDown()
        }
    })
    $("#readerID").change(function () {
        init_Char_Grid();
    })

    getNodeids();
    map = new BMap.Map("allmap"); //创建百度地图实例，这里的allmap是地图容器的id

})


datacache = [];
datacache.records = [];
param = {};
datas = [];
idArray = [];
idArray1 = [];
nodeArray = [];
show = 0;
pages = 0;

function sliderPic(data) {
    data.records.reverse();
    if (data.records[0]) {
        $("#banner_tabs .slides li img").eq(0).attr("style", "background: url(" + data.records[0].image + ");height:100%;background-position:center;background-size:cover")
        if (data.records[0].imageDistinguish) {
            var imageDistinguish = [];
            data.records[0].imageDistinguish.forEach(function (element) {
                if (element.value != "其他") {
                    imageDistinguish.push(element.value + "(" + element.confidence + "%" + ")")
                }
            })
            imageDistinguish = imageDistinguish.join("、");
            $("#banner_tabs .slides li p span").eq(0).text(imageDistinguish);
        }
    } else {
        $("#banner_tabs .slides li img").eq(0).attr("style", "background: url();height:100%;background-position:center;background-size:cover")
    }
    if (data.records[1]) {
        $("#banner_tabs .slides li img").eq(1).attr("style", "background: url(" + data.records[1].image + ");height:100%;background-position:center;background-size:cover")
        if (data.records[1].imageDistinguish) {
            var imageDistinguish = [];
            data.records[1].imageDistinguish.forEach(function (element) {
                if (element.value != "其他") {
                    imageDistinguish.push(element.value + "(" + element.confidence + "%" + ")")
                }
            })
            imageDistinguish = imageDistinguish.join("、");
            $("#banner_tabs .slides li p span").eq(1).text(imageDistinguish);
        }
    } else {
        $("#banner_tabs .slides li img").eq(1).attr("style", "background: url();height:100%;background-position:center;background-size:cover")
    }
    if (data.records[2]) {
        $("#banner_tabs .slides li img").eq(2).attr("style", "background: url(" + data.records[2].image + ");height:100%;background-position:center;background-size:cover")
        if (data.records[2].imageDistinguish) {
            var imageDistinguish = [];
            data.records[2].imageDistinguish.forEach(function (element) {
                if (element.value != "其他") {
                    imageDistinguish.push(element.value + "(" + element.confidence + "%" + ")")
                }
            })
            imageDistinguish = imageDistinguish.join("、");
            $("#banner_tabs .slides li p span").eq(2).text(imageDistinguish);
        }
    } else {
        $("#banner_tabs .slides li img").eq(2).attr("style", "background: url();height:100%;background-position:center;background-size:cover")
    }
}
//获取节点ID
function getNodeids() {
    $.ajax({
        headers: {
            "username": "lora",
            "password": "lora2016",
            "Content-Type": "application/json"
        },
        type: "get",
        dataType: "json",
        url: "http://loradata.handsometechs.com/api/fakenotifyids",
        success: function (ids) {
            nodeArray = ids.notifyids;
            for (i = 0; i < ids.notifyids.length; i++) {
                $("#readerID").append("<option>" + ids.notifyids[i].substring(12, 16) + "</option>");

            }
            select_data_id()

        },
        error: function (error) {
            $(".index").css({
                "display": "none"
            });
        }

    });

}


function select_data_id() {
    $.ajax({
        headers: {
            "username": "lora",
            "password": "lora2016",
            "Content-Type": "application/json"
        },
        type: "get",
        dataType: "json",
        url: "http://loradata.handsometechs.com/api/fakehistory",
        data: { "sTime": Math.round(new Date().getTime() / 1000) - 3600, "datatype": "json" },
        success: function (data) {
            if (data.records && data.records.length != 0) {
                for (var i = data.records.length - 1; i >= 0; i--) {
                    if (data.records[i]["humidity"] > 0 && data.records[i]["temperature"] > -20) {
                        var dataID = data.records[i]["nodeId"];
                        for (var k = 0; k < $("#readerID option").length; k++) {
                            if ($("#readerID option").eq(k).text() == dataID.substring(12, 16)) {
                                $("#readerID option").eq(k).attr("selected", true);
                                break;
                            }
                        }
                        break;
                    }
                }

            }

            init_Char_Grid();
            Scocket();

        },
        error: function (error) {
            $(".index").css({
                "display": "none"
            });
        }
    });
}

//websocket

function Scocket() {
    var socket = new WebSocket('ws://loradata.handsometechs.com/api/websocket');
    // 打开Socket
    socket.onopen = function (event) {
        //console.log("open");
        // 发送一个初始化消息
        socket.send(JSON.stringify({
            username: "lora",
            password: "lora2016"
        }));
        setInterval(function () {
            socket.send("");
        }, 20000)

        // 监听消息
        socket.onmessage = function (event) {
            //console.log('Client received a message', event);
            //                    changedata(event.data);
            var lastdata = {};
            if (event.data.indexOf("{") >= 0 && (JSON.parse(event.data)).nodeId.substring(12, 16) == $("#readerID").val()) {
                var newdata = JSON.parse(event.data);
                for (let prop in newdata) {
                    lastdata[prop] = newdata[prop]
                }
                lastdata["time"] = (new Date()).toString().substring(0, 33);
                BaiduMap(lastdata);
                yibiaoshow(lastdata);
                if (lastdata["image"]) {
                    datacache.records.push(lastdata);
                    sliderPic(datacache);

                }
                // bindGridData(datacache);
                // bindCharData(datacache);
            }

        };

        //error处理
        socket.onerror = function (event) {
            //                    changedata([]);
            //console.log('error');
            Scocket();
        };
        // 监听Socket的关闭
        socket.onclose = function (event) {
            //console.log('Client notified socket has closed', event);
            Scocket();
        };

        // 关闭Socket....
        //socket.close()
    };

}

//    地图显示多个节点
// function pointsMap() {
//     param = {
//         "sTime": Math.round(new Date().getTime() / 1000) - 6000
//     };
//     Get_Char_Grid(param, function (data) {
//         init_Char_Grid();
//     })

// }


function init_Char_Grid() {
    datas = [];
    pages = 0;
    param = {
        "sTime": getDate("#datepicker1"),
        "readerid": getID(),
        "datatype": "json"
    }
    load_Char_Grid(param);

}

function load_Char_Grid(param, go) {
    sTime = getDate("#datepicker1");
    eTime = getDate("#datepicker2");
    if (sTime >= eTime) { } else {
        $(".index").css({
            "display": "block"
        });
        Get_Char_Grid(param, function (data) {
            datacache = JSON.parse(JSON.stringify(data));
            if (getID()[0].indexOf("02000001") >= 0) {
                $(".Chars").css("display", "none");
                $(".grid").css("display", "none");
                $(".map_right").css("display", "none");
                $(".pictures").css("display", "block");
                sliderPic(data);
                if (data.records.length != 0) {
                    var lastdata = data.records[data.records.length - 1];
                    BaiduMap(lastdata);
                } else {
                    var lastdata = "null";
                    mapPosition(lastdata);
                }
                $(".index").css("display", "none");
            } else {
                $(".pictures").css("display", "none");
                $(".Chars").css("display", "block");
                $(".grid").css("display", "block");
                $(".map_right").css("display", "block");
                bindGridData(data);
                bindCharData(data);
                if (data.records.length != 0) {
                    var lastdata = data.records[data.records.length - 1];
                    BaiduMap(lastdata);
                    yibiaoshow(lastdata);
                } else {
                    var lastdata = "null";
                    mapPosition(lastdata);
                    yibiaoshow(lastdata);
                }
            }

        });
    }
}
//调取Char和Grid接口数据
function Get_Char_Grid(param, callback) {
    sTime = getDate("#datepicker1");
    eTime = getDate("#datepicker2");
    var req = {};
    req.url = "http://loradata.handsometechs.com/api/fakehistory";
    req.type = "GET";
    req.dataType = "json";
    req.callBack = callback;
    req.param = param;
    $.ajax({
        headers: {
            "username": "lora",
            "password": "lora2016",
            "Content-Type": "application/json"
        },
        type: req.type,
        dataType: req.dataType,
        url: req.url,
        data: req.param,
        success: function (data) {
            if (typeof (data) != "object" && data != "") {
                data = eval('(' + data + ')');
            }
            datas[pages] = data;
            if (datas[pages].records == "") {
                req.callBack(datas[0]);
                return 0;
            }
            if (eTime < (new Date(datas[pages].records[datas[pages].records.length - 1][1]).getTime().toString().substring(0, 10)) || datas[pages].records.length < 1000) {
                if (pages != 0) {
                    datas[pages].records = datas[pages].records.splice(1, datas[pages].records.length - 1);
                }
                for (i = 0; i < pages; i++) {
                    datas[0].records = $.merge(datas[0].records, datas[i + 1].records)
                }
                for (j = datas[0].records.length - 1; j >= 0; j--) {
                    if (eTime < (new Date(datas[0].records[j]["time"]).getTime().toString().substring(0, 10))) {
                        datas[0].records.splice(j, 1);
                    }
                }
                req.callBack(datas[0]);
            } else {
                param = {
                    "sTime": new Date(datas[pages].records[datas[pages].records.length - 1][1]).getTime().toString().substring(0, 10),
                    "readerid": getID()
                }
                pages++;
                Get_Char_Grid(param, callback)
            }
        },
        error: function (error) {
            $(".index").css({
                "display": "none"
            });
            // if (show == 0) {
            //     show = 1;
            //     $(".tips").click();
            //     //    安全证书
            //     safe();
            // }
        }
    });
}

//加载Grid数据
function bindGridData(data) {
    var data = data.records;
    var data_grid = [];
    for (i = 0; i < data.length; i++) {
        data_grid[i] = {};
        data_grid[i]["nodeID"] = data[i]["nodeId"].substring(12, 16);
        data_grid[i]["sTime"] = data[i]["time"];
        data_grid[i]["Temp"] = data[i]["temperature"];
        data_grid[i]["Humi"] = data[i]["humidity"];
        var time1 = data_grid[i]["sTime"].split(" ");
        var time2 = time1[1] + " " + time1[2] + " " + time1[3] + " " + time1[4];
        data_grid[i]["sTime"] = time2;


    }
    var data_grid1 = [];
    for (i = 0; i < data_grid.length; i++) {
        data_grid1[i] = data_grid[data_grid.length - 1 - i];
    }
    $('#container1').datagrid({
        rowStyler: function (index, row) {
            //		if (index%2==0){
            //			return 'background-color:#6293BB;color:#fff;';
            //		}
        },
        data: data_grid1,
        //    frozenColumns:[[
        //    ]],
        columns: [[
            {
                field: 'nodeID',
                title: 'nodeID',
                'width': '25%',
                halign: 'center',
                align: 'center'
            },
            {
                field: 'Temp',
                title: 'Temperature(℃)',
                'width': '25%',
                halign: 'center'
            },
            {
                field: 'Humi',
                title: 'Humidity(RH)',
                'width': '25%',
                halign: 'center'
            },
            {
                field: 'sTime',
                title: 'Acquisition Time',
                'width': '25%',
                halign: 'center'
            }
        ]]
    });
}

//加载Char数据
function bindCharData(data) {
    var i = 0;
    var options = {

        chart: {
            zoomType: 'x',
            type: 'line',
        },
        title: {
            text: '',
            x: -20
        },
        subtitle: {
            text: '',
            x: -20
        },
        xAxis: {
            categories: [],
            minTickInterval: 80,
        },
        yAxis: [{
            //            min: 15,
            title: {
                text: 'Temperature/℃',
                align: 'high'

            },
            offset: 0,
            lineColor: '#8FC41E',
            lineWidth: 1,
            gridLineWidth: 0,
            showEmpty: false,
            labels: {},
            plotLines: [{}]
        }, {
            title: {
                text: 'Humidity/RH',
                align: 'high'

            },
            offset: 0,
            lineColor: '#14B5B0',
            lineWidth: 1,
            gridLineWidth: 0,
            showEmpty: false,
            opposite: true,
            labels: {},
            plotLines: [{}]
        }],
        tooltip: {
            useHTML: true,
            //            formatter: function () {
            //                return this.x + '</br><span style="color:#14B5C1">' + this.series.name + ':</span>' + '<span>' + standardData("Outside China", this.y, rowIndex) + '</span>';
            //            }
        },
        legend: {
            enabled: true,
            borderWidth: "0",
            //            labelFormatter: legend
        },
        plotOptions: {
            line: {
                pointPadding: 0.2,
                borderWidth: 0,
                cursor: 'pointer',
                events: {
                    click: function (e) { }
                },
                dataLabels: {
                    enabled: false,
                    rotation: -60,
                    series: {
                        datalable: {
                            x: 9,
                            y: 0
                        }
                    }
                }
            },
            series: []
        },
        series: []
    }
    //数据结构转换
    var data_char = [],
        data_x = [];
    for (i = 0; i < 2; i++) {
        data_char[i] = {};
        data_char[i]["Data"] = [];
    }
    data_char[0]["Name"] = "空气温度/Temp.";
    data_char[1]["Name"] = "空气湿度/H";
    data = data.records;
    for (i = 0; i < data.length; i++) {
        var arr1 = data[i]["time"].split(" ");
        data_x[i] = arr1[1] + " " + arr1[2] + " " + arr1[4];
        data_char[0]["Data"][i] = data[i]["temperature"];
        data_char[1]["Data"][i] = data[i]["humidity"];

    }
    options.xAxis.categories = data_x;
    for (i = 0; i < data_char.length; i++) {
        options.series[i] = '{data:' + '[' + data_char[i].Data + ']' + ',yAxis:' + i + ',name:"' + data_char[i].Name + '",lineWidth: 1' + ',color:"' + colorsItem.common[i] + '"}';
        //        if (i % 2 == 0) {
        //            options.series[i] = '{data:' + '[' + data_char[i].Data + ']' + ',name:"' + data_char[i].Name + '",color:"' + colorsItem.common[i] + '"}';
        //        } else {
        //                options.series[i] = '{data:' + '[' + data_char[i].Data + ']' + ',name:"' + data_char[i].Name + '",color:"' + colorsItem.common[i] + '",visible: ' + false + '}';
        //            }
        options.series[i] = eval('(' + options.series[i] + ')')
    }
    if (options.series.length <= 0) {
        options.series = [];
    }
    var chart = $('#container').highcharts(options).highcharts();
    $(".index").css({
        "display": "none"
    });
}

var colorsItem = {
    common: ["#8FC41E", "#14B5B0", '#2f7ed8', '#1aadce', '#7cb5ec', '#90ed7d', '#f7a35c', '#8085e9', '#e4d354', '#2b908f', '#91e8e1', '#492970', '#f28f43', '#99CCFF', '#99FFFF', '#66FFCC', '#66CCFF', '#FF9900', '#CCCCFF', "#8FC41E", "#14B5B0", '#2f7ed8', '#1aadce', '#7cb5ec', '#90ed7d', '#f7a35c', '#8085e9', '#e4d354', '#2b908f', '#91e8e1', '#492970', '#f28f43', '#99CCFF', '#99FFFF', '#66FFCC', '#66CCFF', '#FF9900', '#CCCCFF'],
    blueChange: ['#95B3D7', '#799ECB', '#5C8BC3', '#497BB8', '#3A6EAD', '#3164A1', '#285B99', '#1F5494', '#0E4990', '#08438A']
}

//设置初始日期
function setDate() {
    var setDate = getBeforeDate(0);
    $("#datepicker1").val(setDate);
    var setDate = getBeforeDate(0);
    $("#datepicker2").val(setDate);
}

//获取日期
function getDate(datepicker) {
    var newstr = $(datepicker).val();
    newstr = newstr.replace(/-/g, '/');
    if (datepicker == "#datepicker1") {
        var date = new Date(newstr);
        var time_str = date.getTime().toString();
        return time_str.substring(0, 10);
    } else {
        var date = new Date();
        var date1 = date.getMonth() + 1 + "/" + date.getDate() + "/" + date.getFullYear();
        var arr = newstr.split("/");
        if (date1 == newstr || (arr[2] > date.getFullYear()) || (arr[2] == date.getFullYear() && arr[0] > (date.getMonth() + 1)) || (arr[2] == date.getFullYear() && arr[0] == (date.getMonth() + 1) && arr[1] > date.getDate())) {
            return date.getTime().toString().substring(0, 10)
        } else {
            var date = new Date(newstr);
            var time_str = (date.getTime() + 86399000).toString();
            return time_str.substring(0, 10);
        }
    }
}

//获取n天前的日期
function getBeforeDate(n) {
    var n = n;
    var d = new Date();
    var year = d.getFullYear();
    var mon = d.getMonth() + 1;
    var day = d.getDate();
    if (day <= n) {
        if (mon > 1) {
            mon = mon - 1;
        } else {
            year = year - 1;
            mon = 12;
        }
    }
    d.setDate(d.getDate() - n);
    year = d.getFullYear();
    mon = d.getMonth() + 1;
    day = d.getDate();
    s = (mon < 10 ? ('0' + mon) : mon) + "/" + (day < 10 ? ('0' + day) : day) + "/" + year;
    return s;
}

//获取ID
function getID() {
    //    return [parseInt($("#readerID").val())]
    var arr = [];
    for (var i = 0; i < nodeArray.length; i++) {
        if (nodeArray[i].substring(12, 16) == $("#readerID").val()) {
            arr.push(nodeArray[i]);
            break;
        }
    }
    //    var arr=$("#readerID").val();
    return arr;
}

//安装安全证书
function safe() {
    var localurl = location.href;
    localurl = "http://loradata.handsometechs.com?redirecturl=" + localurl;
    $("#certificate_url").attr("href", localurl);
}


//百度地图定位
// 百度地图API功能
function BaiduMap(data) {
    var id = data["nodeId"];
    if (data["image"]) {
        id = "image"
    }
    if (data["nodeId"].indexOf("2132") >= 0) {
        id = "2132"
    }
    if (data["nodeId"].indexOf("2133") >= 0) {
        id = "2133"
    }
    if (data["nodeId"].indexOf("2134") >= 0) {
        id = "2134"
    }
    switch (id) {
        case "2132":
            mapPosition({ x: "120.213547", y: "30.195306" }, data["nodeId"], "无", "无");
            break;
        case "2133":
            mapPosition({ x: "120.213646", y: "30.194374" }, data["nodeId"], "无", "无");
            break;
        case "2134":
            mapPosition({ x: "120.217284", y: "30.195548" }, data["nodeId"], "无", "无");
            break;
        case "image":
            mapPosition({ x: "120.213651", y: "30.192817" }, data["nodeId"]);
            break;
        case "000000040000ff00":
            mapPosition({ x: "120.214522", y: "30.194037" }, id);
            break;
        case "000000040000ff01":
            mapPosition({ x: "120.223352", y: "30.195938" }, id);
            break;
        case "000000040000ff02":
            mapPosition({ x: "120.219467", y: "30.195192" }, id);
            break;
        case "000000040000ff03":
            mapPosition({ x: "120.214491", y: "30.193967" }, id);
            break;
        case "000000040000ff04":
            mapPosition({ x: "120.219175", y: "30.194006" }, id);
            break;
        case "000000040000ff05":
            mapPosition({ x: "120.222171", y: "30.196289" }, id);
            break;
        default:
            var mapparam = {
                "coords": data["longtitude"] + "," + data["latitude"],
                "from": 1,
                "to": 5,
                "ak": "MLn2wIl0Ft5RfL276PBOKacONXlDbY5H"
            };
            if (data["longtitude"] == 0 || data["latitude"] == 0) {
                mapPosition("null");
            } else {
                //    坐标转换
                $.ajax({
                    type: 'get',
                    async: false,
                    url: 'http://api.map.baidu.com/geoconv/v1/',
                    dataType: 'jsonp',
                    data: mapparam,
                    success: function (data) {
                        var coordinate = data.result[0];
                        mapPosition(coordinate, id)
                    }
                })

            }

    }
}

function mapPosition(data, id) {
    //        if ($("#go").hasClass("gone")) {} else {
    //            map = new BMap.Map("allmap"); //创建百度地图实例，这里的allmap是地图容器的id
    //        }
    // if (idArray.indexOf(id) < 0) {
    var allOverlay = map.getOverlays();
    for (var i = 0; i <= allOverlay.length - 1; i++) {
        map.removeOverlay(allOverlay[i]);
    }
    if (data == "null") {
        return;
    }
    idArray.push(id);
    label = "<div style='line-height:20px;'>" + "节点ID:" + "<br/>" + id.substring(12, 16) + "</div>";
    var point = new BMap.Point(data.x, data.y);
    map.centerAndZoom(point, 16);
    var marker = new BMap.Marker(point);
    var infoWindow = new BMap.InfoWindow(label); // 创建信息窗口对象
    map.addOverlay(marker);
    marker.addEventListener("click", function () {
        this.openInfoWindow(infoWindow);
    });
    map.enableScrollWheelZoom(true);
    // } else {
    //     var point = new BMap.Point(data.x, data.y);
    //     map.centerAndZoom(point, 16);
    // }
}

function yibiaoshow(data) {
    $(".chart-svg path").remove();
    $(".chart-svg polygon").remove();
    drawLineGraph('#chart-1', chart_1_y, '#graph-1-container', 1);
    drawLineGraph('#chart-2', chart_2_y, '#graph-2-container', 2);
    if (data == "null") {
        $("#temp").text("无");
        $("#humi").text("无");
        return;
    }
    var value1 = $("#temp").text();
    if (value1 == "" || value1 == "无") { value1 = 0; }
    var endVal1 = data["temperature"];
    var numAnim1 = new CountUp("temp", value1, endVal1, 1, 4);
    numAnim1.start(function () {
        numAnim1.update(endVal1);
    });
    var value2 = $("#humi").text();
    if (value2 == "" || value2 == "无") { value2 = 0; }
    var endVal2 = data["humidity"];
    var numAnim2 = new CountUp("humi", value2, endVal2, 0, 4);
    numAnim2.start(function () {
        numAnim2.update(endVal2);
    });


}

//仪表盘函数

function board(id, val, zhibiao, danwei, credit, min, max) {
    var val0 = (val - min) * 500 / (max - min) + 400;

    var canvas = document.getElementById(id),
        ctx = canvas.getContext('2d'),
        cWidth = canvas.width,
        cHeight = canvas.height,
        score = val0,
        stage = ['较差', '中等', '良好', '优秀', '极好'],
        radius = 120,
        deg0 = Math.PI / 9,
        deg1 = Math.PI * 11 / 45;

    //  if (score < 400 || score > 900) {
    //    alert('信用分数区间：400~900');
    //  } else {
    console.log(cWidth);
    var dot = new Dot(),
        dotSpeed = 0.07,
        textSpeed = Math.round(dotSpeed * 2000 / deg1),
        angle = 0;
    //        credit = 400;

    (function drawFrame() {

        ctx.save();
        ctx.clearRect(0, 0, cWidth, cHeight);
        ctx.translate(cWidth / 2, cHeight / 2);
        ctx.rotate(8 * deg0);

        dot.x = radius * Math.cos(angle);
        dot.y = radius * Math.sin(angle);

        var aim = (score - 400) * deg1 / 100;
        if (angle < aim) {
            angle += dotSpeed;
        }
        dot.draw(ctx);

        if (credit < val - textSpeed) {
            credit += textSpeed;
        } else if (credit >= val - textSpeed && credit < val) {
            if (val - credit <= 1) {
                credit = val;
            } else {
                credit += 1;
            }
        }
        text(credit);

        //      ctx.save();
        //      ctx.beginPath();
        //      ctx.lineWidth = 3;
        //      ctx.strokeStyle = 'rgba(255, 255, 255, .5)';
        //      ctx.arc(0, 0, radius, 0, angle, false);
        //      ctx.stroke();
        //      ctx.restore();

        window.requestAnimationFrame(drawFrame);

        ctx.save(); //中间刻度层
        ctx.beginPath();
        ctx.strokeStyle = 'rgba(255, 255, 255, .3)';
        ctx.lineWidth = 12;
        ctx.arc(0, 0, 100, 0, 11 * deg0, false);
        ctx.stroke();
        ctx.restore();

        ctx.save(); // 刻度线
        for (var i = 0; i < 6; i++) {
            ctx.beginPath();
            ctx.lineWidth = 4;
            ctx.strokeStyle = 'rgba(255, 255, 255, .5)';
            ctx.moveTo(106, 0);
            ctx.lineTo(94, 0);
            ctx.stroke();
            ctx.rotate(deg1);
        }
        ctx.restore();

        ctx.save(); // 细分刻度线
        for (i = 0; i < 25; i++) {
            if (i % 5 !== 0) {
                ctx.beginPath();
                ctx.lineWidth = 4;
                ctx.strokeStyle = 'rgba(255, 255, 255, .4)';
                ctx.moveTo(106, 0);
                ctx.lineTo(90, 0);
                ctx.stroke();
            }
            ctx.rotate(deg1 / 5);
        }
        ctx.restore();

        //      ctx.save(); //信用分数
        //      ctx.rotate(Math.PI / 2);
        //      for (i = 0; i < 6; i++) {
        //        ctx.fillStyle = 'rgba(255, 255, 255, .4)';
        //        ctx.font = '10px Microsoft yahei';
        //        ctx.textAlign = 'center';
        //        ctx.fillText(400 + 100 * i, 0, -35);
        //        ctx.rotate(deg1);
        //      }
        //      ctx.restore();
        //
        //      ctx.save(); //分数段
        //      ctx.rotate(Math.PI / 2 + deg0);
        //      for (i = 0; i < 5; i++) {
        //        ctx.fillStyle = 'rgba(255, 255, 255, .4)';
        //        ctx.font = '10px Microsoft yahei';
        //        ctx.textAlign = 'center';
        //        ctx.fillText(stage[i], 5, -35);
        //        ctx.rotate(deg1);
        //      }
        //      ctx.restore();

        ctx.save(); //信用阶段及评估时间文字
        ctx.rotate(10 * deg0);
        ctx.fillStyle = '#fff';
        ctx.font = '30px Microsoft yahei';
        ctx.textAlign = 'center';
        //      if (score < 500) {
        //        ctx.fillText('信用较差', 0 , 40);
        //      } else if (score < 600 && score >= 500) {
        //        ctx.fillText('信用中等', 0 , 40);
        //      } else if (score < 700 && score >= 600) {
        //        ctx.fillText('信用良好', 0 , 40);
        //      } else if (score < 800 && score >= 700) {
        //        ctx.fillText('信用优秀', 0 , 40);
        //      } else if (score <= 900 && score >= 800) {
        //        ctx.fillText('信用极好', 0 , 40);
        //      }
        ctx.fillText(zhibiao, 0, 100);
        //      ctx.fillStyle = '#80cbfa';
        //      ctx.font = '14px Microsoft yahei';
        //      ctx.fillText('评估时间：2016.11.06', 0, 60);

        ctx.fillStyle = '#7ec5f9';
        ctx.font = '28px Arial Narrow';
        ctx.fillText(danwei, 0, -40);
        ctx.restore();


        //       ctx.save(); //最外层轨道
        ctx.beginPath();
        ctx.strokeStyle = 'rgba(255, 255, 255, .5)';
        ctx.lineWidth = 6;
        ctx.arc(0, 0, 120, 0, 11 * deg0, false);
        ctx.stroke();
        ctx.restore();

    })();
    //  }

    function Dot() {
        this.x = 0;
        this.y = 0;
        this.draw = function (ctx) {
            ctx.save();
            ctx.beginPath();
            ctx.fillStyle = 'rgba(255, 255, 255, .7)';
            ctx.arc(this.x, this.y, 5, 0, Math.PI * 2, false);
            ctx.fill();
            ctx.restore();
        };
    }

    function text(process) {
        ctx.save();
        ctx.rotate(10 * deg0);
        ctx.fillStyle = '#fff';
        ctx.font = '66px Microsoft yahei';
        ctx.textAlign = 'center';
        ctx.textBaseLine = 'top';
        ctx.fillText(process, 0, 30);
        ctx.restore();
    }
};
