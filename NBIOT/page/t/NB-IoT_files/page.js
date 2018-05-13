$(function () {
    param = {};
    datas = [];
    idArray = [];
    idArray1 = [];
    show = 0;
    pages = 0;
    map = new BMap.Map("allmap"); //创建百度地图实例，这里的allmap是地图容器的id
    setDate();
    //获取节点ID
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
            for (i = 0; i < ids.notifyids.length; i++) {
                $("#readerID").append("<option>" + ids.notifyids[i] + "</option>");

            }
            pointsMap();

        },
        error: function (error) {
            $(".index").css({
                "display": "none"
            });
        }

    });

        //    地图显示多个节点
    function pointsMap() {
        param = {
            "sTime": Math.round(new Date().getTime() / 1000) - 36000
        };
        Get_Char_Grid(param, function (data) {
            for (var i = 0; i < data.records.length; i++)
                if (idArray1.indexOf(data.records[i][0]) < 0) {
                    idArray1.push(data.records[i][0]);
                    BaiduMap(data.records[i]);
                }
            init_Char_Grid();
        })

    }


});


function init_Char_Grid() {
    datas = [];
    pages = 0;
    param = {
        "sTime": getDate("#datepicker1"),
        "readerid": '000000040000ff00'
    }
    load_Char_Grid(param);

}

function load_Char_Grid(param) {
        sTime = getDate("#datepicker1");
        eTime = getDate("#datepicker2");
        if (sTime >= eTime) {} else {
            $(".index").css({
                "display": "block"
            });
            Get_Char_Grid(param, function (data) {
                var lastdata = data.records[data.records.length - 1];
                bindGridData(data);
                bindCharData(data);
                if (lastdata) {
                    BaiduMap(lastdata);
                    yibiaoshow(lastdata);

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
                    if (eTime < (new Date(datas[0].records[j][1]).getTime().toString().substring(0, 10))) {
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
        }
    });
}

//加载Grid数据
function bindGridData(data) {
    var data = data.records;
    var data_grid = [];
    for (i = 0; i < data.length; i++) {
        data_grid[i] = {};
        data_grid[i]["nodeID"] = data[i][0];
        data_grid[i]["sTime"] = data[i][1];
        data_grid[i]["Lat"] = data[i][4];
        data_grid[i]["Long"] = data[i][7] + 110;
        data_grid[i]["Att"] = data[i][6];
        //data_grid[i]["Temp"] = data[i][5];
        //data_grid[i]["Humi"] = data[i][6];
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
                title: '设备编号',
                'width': '20%',
                halign: 'center',
                align: 'center'
            },
            {
                field: 'Lat',
                title: '纬度',
                'width': '20%',
                halign: 'center'
            },
            {
                field: 'Long',
                title: '经度',
                'width': '20%',
                halign: 'center'
            },
            {
                field: 'Att',
                title: '海拔',
                'width': '20%',
                halign: 'center'
            }/*,
            {
                field: 'Temp',
                title: '温度(℃)',
                'width': '14%',
                halign: 'center'
            },
            {
                field: 'Humi',
                title: '湿度(RH)',
                'width': '14%',
                halign: 'center'
            }*/,
            {
                field: 'sTime',
                title: '记录时间',
                'width': '20%',
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
                    text: '移动情况/Speed',
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
                min: 0,
                title: {
                    text: '距离/M',
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
                        click: function (e) {}
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
    data_char[0]["Name"] = "移动速度/Speed.";
    data_char[1]["Name"] = "距离/M";
    data = data.records;
    for (i = 0; i < data.length; i++) {
        var arr1 = data[i][1].split(" ");
        data_x[i] = arr1[1] + " " + arr1[2] + " " + arr1[4];
        data_char[0]["Data"][i] = data[i][5];
        data_char[1]["Data"][i] = data[i][7];

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

$(document).ready(function () {
        //帮助
        // $(".tips").on("click", function () {
        //         $(".safety").toggle();
        //         $(".tips span").css({
        //             "color": "#551A8B"
        //         });
        //         safe();
        //     })
            //日历
        $('#datepicker1').datepicker();
        $('#datepicker2').datepicker();
        $("#go").mousedown(function () {
            $("#go").css({
                "background-color": "#C02020"
            })
        })
        $("#go").mouseup(function () {
            $("#go").css({
                "background-color": "#FF0000"
            })
        })

        //提交
        $("#go").on("click", function () {
                datas = [];
                pages = 0;
                $("#go").addClass("gone");
                param = {
                    sTime: getDate("#datepicker1"),
                    "readerid": getID()
                }
                load_Char_Grid(param);
                //    loadChar(param);
            })
            //pic slide
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
        //char grid slide
        $(".title_head").click(function (e) {
            if ($(e.target).siblings().attr("style") == undefined || $(e.target).siblings().attr("style").indexOf("display: none;") < 0) {
                $(e.target).siblings().slideUp()
            } else {
                $(e.target).siblings().slideDown()
            }
        })


    })
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
    arr.push($("#readerID").val());
    //    var arr=$("#readerID").val();
    return arr;
}


//百度地图定位
// 百度地图API功能
function BaiduMap(data) {
    var id = data[0];
    var mapparam = {
        "coords": data["longtitude"] + "," + data["latitude"],
        "from": 1,
        "to": 5,
        "ak": "MLn2wIl0Ft5RfL276PBOKacONXlDbY5H"
    };
    if (data["longtitude"] + "," + data["latitude"]) {
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



    function mapPosition(data, id) {
        //        if ($("#go").hasClass("gone")) {} else {
        //            map = new BMap.Map("allmap"); //创建百度地图实例，这里的allmap是地图容器的id
        //        }
        // if (idArray.indexOf(id) < 0) {
        //     idArray.push(id);
        //     label = "<div style='line-height:20px;'>" + "节点ID:" + "<br/>" + id + "</div>";
        //     var point = new BMap.Point(data.x, data.y);
        //     map.centerAndZoom(point, 16);
        //     var marker = new BMap.Marker(point);
        //     var infoWindow = new BMap.InfoWindow(label); // 创建信息窗口对象
        //     map.addOverlay(marker);
        //     marker.addEventListener("click", function () {
        //         this.openInfoWindow(infoWindow);
        //     });
        //     map.enableScrollWheelZoom(true);
        // } else {
        //     var point = new BMap.Point(data.x, data.y);
        //     map.centerAndZoom(point, 16);
        // }
        var allOverlay = map.getOverlays();
		for (var i = 0; i <= allOverlay.length -1; i++){
			map.removeOverlay(allOverlay[i]);
		}
		if(data=="null"){
			return;
		}
		idArray.push(id);
		label = "<div style='line-height:20px;'>" + "节点ID:" + "<br/>" + id.substring(12,16) + "</div>";
		var point = new BMap.Point(data.x, data.y);
		map.centerAndZoom(point, 16);
		var marker = new BMap.Marker(point);
		var infoWindow = new BMap.InfoWindow(label); // 创建信息窗口对象
		map.addOverlay(marker);
		marker.addEventListener("click", function () {
			this.openInfoWindow(infoWindow);
		});
		map.enableScrollWheelZoom(true);
    }
}

function yibiaoshow(data) {
    $('.map_right .temp .counter').html(data[5]);
    $('.map_right .humi .counter').html(data[6]);
    $('.counter').countUp({
        time:300
    });
}
