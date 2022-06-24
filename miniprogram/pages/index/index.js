// index.js
// 获取应用实例
const app = getApp()
const data={
    "datastreams": [
      {"id": "Change","datapoints":[{"value": "TRUE"}]}
    ]
  }
const devicesId = "859642542"
const api_key = "z51k7ZIQCdjxhtnZKUgZHGIQI1g=" 

Page({
  data:{
      text:"插座正通着电"
  },
  onLoad(){
    var that=this;
    wx.request({
      url: `https://api.heclouds.com/devices/${devicesId}/datapoints?datastream_id=Station,Change&limit=20`,
      header: {
        'content-type': 'application/json',
        'api-key': api_key
      },
      success: (res) => {
        const status = res.statusCode
        const response = res.data
        res=res.data.data.datastreams
        res=res[0]
        res=res.datapoints
        res=res[0]
        res=res.value
        console.log(res)//这里的res就是最新的状态！！！
        if(res=="LOW"){
          that.setData({
              'text':'插座没通电'
          })
          wx.showToast({
            title: '电源是关闭状态',
            duration:500,
            icon:'success',
            mask:true,
          })
        }
        else{
          that.setData({
            'text':'插座通着电'
        })
          wx.showToast({
            title: '电源是关开启态',
            duration:500,
            icon:'success',
            mask:true,
        })
        }
                if (status !== 200) { // 返回状态码不为200时将Promise置为reject状态
                  reject(res.data)
                  return ;
                }
                if (response.errno !== 0) { //errno不为零说明可能参数有误, 将Promise置为reject
                  reject(response.error)
                  return ;
                }
              },
              fail: (err) => {
              }
            })
  },
  SetDatapoints: function () {
    console.log('11111');
    var that=this;
    return new Promise((resolve, reject) => {
      wx.request({
        url: "https://api.heclouds.com/devices/" + devicesId + "/datapoints",
                method:'POST',
                header:{
                  "content-type": 'application/json',
                  "api-key": api_key
                },
                data:JSON.stringify(data),
                success(res){

                  console.log("更新数据成功",res)
                },
                fail: function(res){
                  wx.showToast({ title: '系统错误' })
                },
                complete:function(res){
                  wx.hideLoading()
                }
          
              })

        setTimeout(function(){
          wx.request({
            url: `https://api.heclouds.com/devices/${devicesId}/datapoints?datastream_id=Station,Change&limit=20`,
            header: {
              'content-type': 'application/json',
              'api-key': api_key
            },
            success: (res) => {
              const status = res.statusCode
              const response = res.data
              res=res.data.data.datastreams
              res=res[0]
              res=res.datapoints
              res=res[0]
              res=res.value
              console.log(res)//这里的res就是最新的状态！！！
              if(res=="LOW"){
                that.setData({
                    'text':'插座没通电'
                })
                wx.showToast({
                  title: '电源是关闭状态',
                  duration:500,
                  icon:'success',
                  mask:true,
                })
              }
              else{
                that.setData({
                  'text':'插座通着电'
              })
                wx.showToast({
                  title: '电源是关开启态',
                  duration:500,
                  icon:'success',
                  mask:true,
              })
              }
                      if (status !== 200) { // 返回状态码不为200时将Promise置为reject状态
                        reject(res.data)
                        return ;
                      }
                      if (response.errno !== 0) { //errno不为零说明可能参数有误, 将Promise置为reject
                        reject(response.error)
                        return ;
                      }
            
            
                      //程序可以运行到这里说明请求成功, 将Promise置为resolve状态
                      resolve({
                        temperature: response.data.datastreams[0].datapoints.reverse(),
                        // light: response.data.datastreams[1].datapoints.reverse(),
                        humidity: response.data.datastreams[1].datapoints.reverse()
                      })
                    },
                    fail: (err) => {
                      reject(err)
                    }
                  })
      },10000)

      
  })


},

})
