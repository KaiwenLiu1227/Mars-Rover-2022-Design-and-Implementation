<template>
  <div>
    <!--    <div style="position: absolute;left:0px;top:0px;right: 0px;bottom: 0px; width: 100%;-->
    <!--    height: 100%;" >-->
    <!--    </div>-->

    <el-container @mousemove="mouseMove"
                  @mouseup="mouseEnd">
      <el-main width="400px" style="margin-left: 10px">
        <div
            class="shadedbox" style="height: 220px;margin-bottom: 40px;">
          <el-col style="margin-left: 15px;margin-top: 15px">
            <el-row>
              <el-col :span="7">
                <el-row>
                  <el-switch active-text="Reset" v-model="resetState"/>
<!--                  <el-button style="height: 18px;width: 36px;border-radius: 15px;border: #c9c9c9 solid"></el-button>-->
<!--                  <button style="height: 18px;width: 36px;border-radius: 15px;border: #c9c9c9 solid" onclick=""></button>-->
                </el-row>
                <el-row>
                  <el-col :span="3">
                    <h5 style="">X:</h5>
                  </el-col>
                  <el-col :span="5">
                    <div style="width: 24px;height: 14px;background-color: #cccfd2;border-radius: 15%;margin-top: -3px">
                      <h6>
                        {{ parseInt(x_y_loc[0]) }}</h6></div>
                  </el-col>
                </el-row>
                <el-row style="float: top;margin-top: -30px">
                <el-col :span="3">
                  <h5>Y:</h5>
                </el-col>
                <el-col :span="4">
                  <div
                      style="width: 24px;height: 14px;background-color: #cccfd2;border-radius: 15%;float:top;margin-top: -3px">
                    <h6>{{ parseInt(x_y_loc[1]) }}</h6></div>
                </el-col>
              </el-row>
            </el-col>
            <el-col :span="8">
              <el-row>
                <el-switch active-text="Connect" v-model="connectState" @change="connectHandler"/>
              </el-row>
              <el-row>
                <el-col :span="6">
                  <h5>Yaw:</h5>
                </el-col>
                <el-col :span="4">
                  <div style="width: 24px;height: 14px;background-color: #cccfd2;border-radius: 15%;margin-top: -3px">
                    <h6>
                      {{ (parseInt(180*yaw/Math.PI)) }}</h6></div>
                </el-col>
              </el-row>
              <el-row style="float: top;margin-top: -30px">
                <el-col :span="6">
                  <h5>Alien:</h5>
                </el-col>
                <el-col :span="4">
                  <div
                      style="width: 24px;height: 14px;background-color: #cccfd2;border-radius: 15%;float:top;margin-top: -3px">
                    <h6>{{ alien_list.length }}</h6></div>
                </el-col>
              </el-row>
            </el-col>
            <el-col :span="8">
              <el-row>
                <el-switch :active-text="ModeDisplay" v-model="driveState" @change="modeSwitchHandler"/>
              </el-row>
              <el-row style="">
                <h5>Sensor Quality:</h5>
              </el-row>
              <el-row style="margin-top: -10px">
                <div
                    style="width: 24px;height: 14px;background-color: #cccfd2;border-radius: 15%; margin-left: 30px">
                  <h6 style="float: top;margin-top: 0px">{{ sensor_quality }}</h6></div>
              </el-row>
            </el-col>
          </el-row>
          <el-row>
            <el-col span="4" style="background-color: rgba(255,255,255,0.18); border-radius: 10px;padding-top: 10px;margin-top: -5px;padding-bottom: 5px;padding-left: 5px;margin-left: -5px">
              <el-row>
                <div class="text-container" style="overflow: hidden;word-break: break-all">
                  <p class="text" v-for="value in pushList()" :key="value.id">{{ value.text }}</p>
                </div>
              </el-row>
              <el-row>
                <el-input
                    style="width: 200px;height: 24px;"
                    placeholder="Enter command line"
                    v-model="cmd_input"
                    clearable>
                </el-input>
              </el-row>
            </el-col>
            <el-col span="4">
              <h5 style="margin-top: 15px;margin-left: 30px">Yaw:</h5>
            </el-col>
            <el-col span="6">
              <div
                  style="height: 60px; width: 60px; border-radius: 100%; margin-top: -10px;margin-left: 10px;"
                  class="shadedbox">
                <img v-if="connectState" alt="" src="../assets/arrow_on.png"
                     style="object-fit: scale-down;  height: 50px;width: 50px;margin-top: 5px"
                     v-bind:style="{transform: 'rotate('+ ((yaw*180)/Math.PI+180) +'deg)'}">
                <img v-if="!connectState" alt="" src="../assets/arrow.png"
                     style="object-fit: scale-down; opacity: 0.6; height: 50px;width: 50px;margin-top: 5px">
              </div>
            </el-col>
          </el-row>
        </el-col>
      </div>
      <div
          class="shadedbox" style="height: 260px; ">
        <el-row style="margin-left: 20px;margin-top: 10px">
          <el-col :span="2" style="margin-right: 20px">
            <div
                class="shadedbox" style="width: 30px;">
              <el-slider
                  v-model="bit_rate"
                  vertical
                  height="170px"
                  style="margin-left: -4px;margin-bottom: 16px;margin-top: 16px;"
              >
              </el-slider>
            </div>
            <h6 style="margin-top: 8px; color: #797979">Motion Intensity</h6>
          </el-col>
          <el-col :span="2" style="margin-right: 20px;" >
            <div
                class="shadedbox" style="width: 30px;">
              <el-slider
                  v-model="bit_rate"
                  vertical
                  height="170px"
                  style="margin-left: -4px;margin-bottom: 16px;margin-top: 16px;"
              >
              </el-slider>
<!--              <div-->
<!--                  style="margin-left: -4px;margin-bottom: 16px;margin-top: 16px;">-->
<!--              </div>-->
            </div>
            <h6 style="margin-top: 8px; color: #797979">Sensor Quality</h6>
          </el-col>
          <el-col :span="2">
            <div
                class="shadedbox" style="width: 30px;">
              <el-slider
                  v-model="bit_rate"
                  vertical
                  height="170px"
                  style="margin-left: -4px;margin-bottom: 16px;margin-top: 16px;"
              >
              </el-slider>

            </div>
            <h5 style="margin-top: 8px; color: #797979">Speed</h5>
          </el-col>
          <el-col :span="8">
            <div class="joystick" id="gamepad" style="  width: 200px; height: 200px; margin-left: 30%;margin-top: 15px"
                 @touchstart="touchstart"
                 @touchmove="touchmove"
                 @touchend="touchend"
                 @mousedown="mouseStart"
            >
              <div v-if="connectState" class="joystick" style="width: 100px; height: 100px; position: relative; background-color: #2989e3"
                   v-bind:style="{left: joystick_x+'px', top: joystick_y+'px'}"
              ></div>
              <div v-if="!connectState" class="joystick" style="width: 100px; height: 100px; position: relative; background-color: #a8b2bb; top:50px;left: 50px"
              ></div>
            </div>
          </el-col>
        </el-row>
      </div>
      </el-main>
      <el-main style="margin: auto;">
        <canvas class="canvas" :width="720" :height="520" id="ctx"
        ></canvas>
      </el-main>
    </el-container>
  </div>
</template>

<script>
export default {
  name: 'RoverMap',
  props: {
    msg: String,
  },
  data() {
    return {
      web_socket: null,
      bootState: false,
      resetState:false,
      connectState: false,
      driveState: false,
      mouse_state: false,
      sensor_quality: 0,
      cmd_input: '',
      speed: 0,
      sensitivity: 0,
      bit_rate: 0,
      xy_str: "",
      x_y_loc: [0, 0],
      yaw: 0,
      gamepad_data: [0, 0, 0, 0],
      alien_list: [],
      joystick_x: 50,
      joystick_y: 50,
      joystick_x_init: 0,
      joystick_y_init: 0,
      m_joystick_x_init: 0,
      m_joystick_y_init: 0,
      moveX: 0,
      moveY: 0,
      dataList: ["initialised", "starting tracing", "hello command center!"],
      ModeDisplay: "Auto Mode"
    }
  },
  mounted() {
    setInterval(
        () => {
          //gamepad test comment following part for real situation
          // this.x_y_loc[0] += this.moveX
          // this.x_y_loc[1] += this.moveY
          // if (this.moveX > 0) {
          //   if (this.moveY > 0) {
          //     this.yaw = -Math.PI / 2 + Math.atan(this.moveX / this.moveY)
          //   } else {
          //     this.yaw = Math.PI / 2 + Math.atan(this.moveX / this.moveY)
          //   }
          // } else if (this.moveY < 0) {
          //   this.yaw = Math.PI / 2 + Math.atan(this.moveX / this.moveY)
          // } else {
          //   this.yaw = -Math.PI / 2 + Math.atan(this.moveX / this.moveY)
          // }

          //monitor
          this.xy_str = [-this.x_y_loc[0]+700,-this.x_y_loc[1]+500].toString()
          this.pushList().push(this.xy_str)
          //gamepad
          if (this.connectState && this.driveState) {
            var x2 = -this.moveX
            var y2 = -this.moveY
            var status_byte = 0
            var left = 0
            var right = 0
            const clamp = (num, min, max) => Math.min(Math.max(num, min), max);
            if (Math.abs(x2) <= 0.01) {
              x2 = Math.sign(x2) * 0.01
            }
            if (x2 === 0 && y2 === 0) {
              right = 0
              status_byte = 0
              left = 0
            } else {
              var mag2 = Math.sqrt(y2 * y2 + x2 * x2)
              if (mag2 <= 20) {
                left=0
                right = 0
              } else {
                var angle2 = 180 / Math.PI * Math.atan(Math.abs(y2) / x2)
              }
              if (angle2 <= 0) {
                angle2 += 180
              }
              if (y2 < -10) {
                status_byte = 0
                right = clamp(parseInt(255 * angle2 / 90), 30, 255)
                left = clamp(parseInt(255 * (2 - angle2 / 90)), 30, 255)
              } else {
                status_byte = 3
                right = clamp(parseInt(255 * angle2 / 90), 30, 255)
                left = clamp(parseInt(255 * (2 - angle2 / 90)), 30, 255)

              }

            }
            var pwn1 = parseInt(left/2)
            var pwn2 =  parseInt(right/2)
            // console.log("%f , %f" ,pwn1, pwn2)
            var cmessage = new Uint8Array([36, 1,status_byte,pwn1,pwn2])
            this.dataList.push("control msg sent: " + cmessage);
            // console.log(cmessage)
            // var string=""
            // for(var i=0;i<cmessage.length;i++){
            //   string+=String.fromCharCode(cmessage[i])
            // }
            this.web_socket.send(String.fromCharCode.apply(null, cmessage))
          }

        },
        150
    )
  },
  watch: {
    xy_str: function (new_XY, old_XY) {
      const canvas = document.getElementById("ctx");
      const ctx = canvas.getContext("2d");

      const newXY = [parseFloat(new_XY.split(",")[0]), parseFloat(new_XY.split(",")[1])]
      const oldXY = [parseFloat(old_XY.split(",")[0]), parseFloat(old_XY.split(",")[1])]

      // erase original arrow
      //eraser 1
      // ctx.moveTo(oldXY[0] + 32 * Math.cos(this.yaw), oldXY[1] - 32 * Math.sin(this.yaw))
      // ctx.lineTo(oldXY[0] + 10 * Math.sin(this.yaw) - 20 * Math.cos(this.yaw), oldXY[1] - 10 * Math.cos(this.yaw) + 20 * Math.sin(this.yaw))
      // ctx.lineTo(oldXY[0] + 2 * Math.cos(this.yaw), oldXY[1] - 2 * Math.sin(this.yaw))
      // ctx.lineTo(oldXY[0] - 10 * Math.sin(this.yaw) + 20 * Math.cos(this.yaw), oldXY[1] + 10 * Math.cos(this.yaw) - 20 * Math.sin(this.yaw))
      // ctx.fillStyle = "#ffffff";
      // ctx.fill()

      //eraser 2
      ctx.fillStyle = "#ffffff";
      ctx.fillRect(oldXY[0] - 5, oldXY[1] - 5, 10, 10);

      ctx.strokeStyle = '#3350e0';

      //draw line

      ctx.lineWidth = 1;
      ctx.moveTo(oldXY[0], oldXY[1]);
      ctx.lineTo(newXY[0], newXY[1]);

      //draw arrow
      // ctx.strokeStyle = '#c70303';
      // ctx.beginPath()
      // ctx.moveTo(newXY[0] + 20 * Math.cos(this.yaw), newXY[1] - 20 * Math.sin(this.yaw))
      // ctx.lineTo(newXY[0] + 2 * Math.sin(this.yaw), newXY[1] + 2 * Math.cos(this.yaw))
      // ctx.lineTo(newXY[0] - 2 * Math.sin(this.yaw), newXY[1] - 2 * Math.cos(this.yaw))
      // ctx.lineTo(newXY[0] + 20 * Math.cos(this.yaw), newXY[1] - 20 * Math.sin(this.yaw))

      //draw square
      ctx.fillStyle = "#6b45a1";
      ctx.fillRect(newXY[0] - 3, newXY[1] - 3, 6, 6);

      ctx.stroke();

      // this.dataList.push("xy: " + parseInt(newXY[0]) + " " + parseInt(newXY[1]));
      // this.dataList.push("yaw: " + this.yaw)
    },
    yaw: function () {

    },
    resetState: function () {
      this.ResetCanvas()
      // for (let i = 0; i < 8; i++) {
      //   this.generateAlian()
      // }
    }
  },
  methods: {
    ResetCanvas(){
      this.resetState=false
      const canvas = document.getElementById("ctx");
      const ctx = canvas.getContext("2d");
      ctx.fillStyle="#ffffff";
      ctx.fillRect(0, 0, canvas.width, canvas.height);
    },
    initWebSocket() {
      const gateway = "ws://192.168.4.1:80/control";
      console.log('Trying to open a WebSocket connection...');
      this.web_socket = new WebSocket(gateway);
      this.web_socket.binaryType="arraybuffer"
      this.web_socket.onopen = this.RegMonitor;
      this.web_socket.onclose = this.onClose;
      this.web_socket.onmessage = this.onMessage;

    },
    RegMonitor() {
      console.log('monitor socket opening');
      this.connectState = true
      this.web_socket.send(String.fromCharCode.apply(null, [36, 0, 0]))
      // this.RegGamepad()
      console.log('monitor socket opened');

    },
    RegGamepad() {
      this.web_socket.send(String.fromCharCode.apply(null, [36, 0, 1]))
      console.log('gamepad socket opened');
    },
    DisRegGamepad(){
      this.web_socket.send(String.fromCharCode.apply(null, [36, 0, 2]))
    },
    RegControl() {
      console.log("control socket opened")
    },
    connectHandler() {
      if (this.connectState) {
        this.initWebSocket()
      } else {
        this.connectState = false
        this.web_socket.close()
      }
    },
    modeSwitchHandler() {
      if (this.driveState) {
        this.ModeDisplay = "Manual Mode"
        this.RegGamepad()
      } else {
        this.ModeDisplay = "Auto Mode"
        this.DisRegGamepad()
      }
    },
    onClose() {
      console.log('Connection closed');
      if (this.connectState) {
        setTimeout(this.initWebSocket, 100);
      }
    },
    onMessage(event) {
      // console.log(event.data.toString())
      this.connectState = true
      this.dataList.push(event.data.toString())
      this.decodeMessage(event.data.toString())
    },
    decodeMessage(msg) {
      // console.log(msg)
      if (msg.indexOf("$opt") !== -1) {
        var x_y_pos = msg.split("real:")[1].split(",")[0].split(" ");
        this.x_y_loc[0] = parseFloat(x_y_pos[0]) * 2;
        this.x_y_loc[1] = parseFloat(x_y_pos[1]) * 2;
        this.yaw = parseFloat(msg.split("yaw:")[1].split(",")[0])
        this.sensor_quality = parseFloat(msg.split("qual:")[1].split(",")[0])
        // console.log("x: %f y: %f yaw: %f q:%f", this.x_y_loc[0], this.x_y_loc[1],this.yaw,this.sensor_quality);
      }
    },
    pushList() {
      const len = this.dataList.length
      return [
        {id: 1, text: this.dataList[len - 2]},
        {id: 2, text: this.dataList[len - 1]}]

    },
    generateAlian() {
      var canvas = document.getElementById("ctx");
      var cxt = canvas.getContext("2d");
      var color
      var rdn_x = Math.random() * 700
      var rdn_y = Math.random() * 500
      switch (parseInt(rdn_y / 100)) {
        case 1:
          color = "#656792";
          break;
        case 2:
          color = "#91bb77";
          break;
        case 3:
          color = "#a76464";
          break;
        case 4:
          color = "#d09f3a";
          break;
        default:
          color = "#9a56bb";
      }

      cxt.fillStyle = color;
      cxt.beginPath();
      cxt.arc(rdn_x, rdn_y, 8, 0, Math.PI * 2, true);
      cxt.closePath();
      cxt.fill();
      this.alien_list.push({x: rdn_x, y: rdn_y, color: color})
      this.dataList.push('alien generated at ' + rdn_x + ' , ' + rdn_y);
    },
    mouseStart() {
      console.log("click down!")
      this.mouse_state = true;
    },
    mouseMove(e) {
      const clamp = (num, min, max) => Math.min(Math.max(num, min), max);
      var rect = document.getElementById('gamepad').getBoundingClientRect()
      var x = e.pageX - rect.x
      var y = e.pageY - rect.y
      if (this.mouse_state&this.driveState) {
        this.joystick_x = clamp(0.5 * (x - this.m_joystick_x_init), -100, 100) + 50
        this.joystick_y = clamp(0.5 * (y - this.m_joystick_y_init), -100, 100) + 50
        this.moveX = (this.joystick_x-50)
        this.moveY = (this.joystick_y-50)
        // console.log("touch moved %d %d", x - this.m_joystick_x_init, y - this.m_joystick_y_init)
      } else {
        this.m_joystick_x_init = x
        this.m_joystick_y_init = y
      }
      // if (this.mouse_state) {
      //   let x = e.offsetX
      //   let y = e.offsetY
      //   this.joystick_x = clamp(x - 75, -100, 100)
      //   this.joystick_y = clamp(y - 100, -100, 100)

      //
      // }
    },
    mouseEnd() {
      console.log("click up!")
      this.joystick_x = 50
      this.joystick_y = 50
      this.moveX = 0
      this.moveY = 0
      this.mouse_state = false;
    },
    touchstart(e) {
      let x = e.touches[0].clientX
      let y = e.touches[0].clientY
      this.joystick_x = 50
      this.joystick_y = 50
      this.joystick_x_init = x
      this.joystick_y_init = y
      this.moveX = 0
      this.moveY = 0
      // console.log("touched %d %d", x, y)
    },
    touchend() {
      this.show = false
      this.joystick_x = 50
      this.joystick_y = 50
      this.moveX = 0
      this.moveY = 0
    },
    touchmove(e) {
      if (this.driveState) {
        let x = e.touches[0].clientX
        let y = e.touches[0].clientY
        let stick_x = x - this.joystick_x_init
        if (stick_x > 150) {
          stick_x = 150
        } else if (stick_x < -50) {
          stick_x = -50
        }
        this.joystick_x = stick_x
        let stick_y = y - this.joystick_y_init
        if (stick_y > 150) {
          stick_y = 150
        } else if (stick_y < -50) {
          stick_y = -50
        }
        this.joystick_y = stick_y
        this.moveX = (this.joystick_x - 75) / 50
        this.moveY = (this.joystick_y - 75) / 50
      }
      // console.log("touch moved %d %d",this.moveX,this.moveY)
    },
  }
}
</script>

<style scoped>
.text-container {
  font-size: 14px;
  color: #929292;
  /*margin-left: 10px;*/
  margin-top: -5px;
  /*margin-top: 40px;*/
  /*height: 150px;*/
  width: 260px;
  height: 48px;
  text-overflow: ellipsis;

}

.text {
  text-align: left;
  margin: auto 0;
}

.shadedbox {
  border-radius: 14px;
  border: 2px solid rgb(199, 198, 198);
  box-shadow: 2px 2px 2px 1px rgba(78, 78, 255, 0.2);
}

.joystick {
  border-radius: 50%;
  border: 1px solid #a8b2bb;
  box-shadow: 2px 2px 2px 1px rgba(78, 78, 255, 0.2);
}

#ctx {
  border: 1px solid rgb(199, 198, 198);
  box-shadow: 2px 2px 2px 1px rgba(74, 74, 255, 0.2);
  border-radius: 3%;
}
</style>
