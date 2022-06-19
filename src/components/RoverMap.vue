<template>
  <el-container>
    <el-main width="400px" style="margin-left: 10px">
      <div
          class="shadedbox" style="height: 220px;margin-bottom: 40px;">
        <el-col style="margin-left: 15px;margin-top: 15px">
          <el-row>
            <el-col :span="7">
              <el-row>
                <el-switch active-text="Reset" v-model="bootState"/>
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
              <el-row style="float: top;margin-top: -20px">
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
                <el-switch active-text="Connect" v-model="connectState" @onclick="initWebSocket"/>
              </el-row>
              <el-row>
                <el-col :span="6">
                  <h5>Yaw:</h5>
                </el-col>
                <el-col :span="4">
                  <div style="width: 24px;height: 14px;background-color: #cccfd2;border-radius: 15%;margin-top: -3px">
                    <h6>
                      {{ (parseInt(yaw)) }}</h6></div>
                </el-col>
              </el-row>
              <el-row style="float: top;margin-top: -20px">
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
                <el-switch active-text="ManualMode" v-model="driveState"/>
              </el-row>
              <el-row>
                <div
                    style="height: 60px; width: 60px; border-radius: 100%; border: 2px solid rgb(199, 198, 198); margin-top: 15px;margin-left: 30px">
                  <div style="width: 60px;height: 60px;"
                       v-bind:style="{transform: 'rotate('+yaw*180/Math.PI+'deg)'}"></div>
                </div>
              </el-row>
            </el-col>
          </el-row>
          <el-row>
            <div class="text-container">
              <p class="text" v-for="value in pushList()" :key="value.id">{{ value.text }}</p>
            </div>
          </el-row>
        </el-col>
      </div>
      <div
          class="shadedbox" style="height: 260px; ">
        <el-row style="margin-left: 20px;margin-top: 10px">
          <el-col span="4" style="margin-right: 20px">
            <div
                class="shadedbox" style="width: 30px;">
              <el-slider
                  v-model="speed"
                  vertical
                  height="200px"
                  style="margin-left: -4px;margin-bottom: 16px;margin-top: 16px;"
              >
              </el-slider>
            </div>
          </el-col>
          <el-col span="4" style="margin-right: 20px">
            <div
                class="shadedbox" style="width: 30px;">
              <el-slider
                  v-model="sensitivity"
                  vertical
                  height="200px"
                  style="margin-left: -4px;margin-bottom: 16px;margin-top: 16px;"
              >
              </el-slider>
            </div>
          </el-col>
          <el-col span="4">
            <div
                class="shadedbox" style="width: 30px;">
              <el-slider
                  v-model="bit_rate"
                  vertical
                  height="200px"
                  style="margin-left: -4px;margin-bottom: 16px;margin-top: 16px;"
              >
              </el-slider>
            </div>
          </el-col>
          <el-col span="50">
            <div class="joystick" style="  width: 200px; height: 200px; margin-left: 30%;margin-top: 15px"
                 @touchstart="touchstart"
                 @touchmove="touchmove"
                 @touchend="touchend"
                 @mousedown="mouseStart"
                 @mousemove="mouseMove"
                 @mouseup="mouseEnd"
            >
              <div class="joystick" style="width: 100px; height: 100px; position: relative; background-color: #a8b2bb"
                   v-bind:style="{left: joystick_x+'px', top: joystick_y+'px'}"
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
</template>

<script>
export default {
  name: 'RoverMap',
  props: {
    msg: String,
  },
  data() {
    return {
      monitor_socket: null,
      bootState: false,
      connectState: false,
      driveState: false,
      mouse_state: false,
      speed: 0,
      sensitivity: 0,
      bit_rate:0,
      xy_str: "",
      x_y_loc: [200, 100],
      yaw: 0,
      gamepad_data: [0, 0, 0, 0],
      alien_list: [],
      joystick_x: 50,
      joystick_y: 50,
      joystick_x_init: 0,
      joystick_y_init: 0,
      moveX: 0,
      moveY: 0,
      dataList: ["initialised", "starting tracing", "hello command center!"]
    }
  },
  mounted() {
    const that = this;

    // this.initWebSocket();

    document.addEventListener('keydown', that.handleWatchEnter);
    const canvas = document.getElementById("ctx");
    const ctx = canvas.getContext("2d");


    setInterval(
        () => {
          //gamepad test
          this.x_y_loc[0] += this.moveX
          this.x_y_loc[1] += this.moveY
          if (this.moveX > 0) {
            if (this.moveY > 0) {
              this.yaw = -Math.PI / 2 + Math.atan(this.moveX / this.moveY)
            } else {
              this.yaw = Math.PI / 2 + Math.atan(this.moveX / this.moveY)
            }
          } else if (this.moveY < 0) {
            this.yaw = Math.PI / 2 + Math.atan(this.moveX / this.moveY)
          } else {
            this.yaw = -Math.PI / 2 + Math.atan(this.moveX / this.moveY)
          }

          //monitor
          this.xy_str = this.x_y_loc.toString()

          //gamepad
          if (this.connectState) {
            var x2 = parseInt(this.moveX * 50)
            var y2 = parseInt(this.moveY * 50)
            const clamp = (num, min, max) => Math.min(Math.max(num, min), max);
            if (Math.abs(x2) <= 0.01) {
              x2 = Math.sign(x2) * 0.01
            }
            // console.log(x2,y2)
            if (x2 == 0 && y2 == 0) {
              right = 0
              status_byte = 0
              left = 0
            } else {
              var mag2 = Math.sqrt(y2 * y2 + x2 * x2)
              if (mag2 <= 20) {
                var left = 0
                var right = 0
                console.log("brake")
              } else {
                var angle2 = 180 / Math.PI * Math.atan(Math.abs(y2) / x2)
              }
              if (angle2 <= 0) {
                angle2 += 180
              }
              if (y2 < -10) {
                var status_byte = 0
                right = clamp(parseInt(255 * angle2 / 90), 30, 255)
                left = clamp(parseInt(255 * (2 - angle2 / 90)), 30, 255)
              } else {
                status_byte = 3
                right = clamp(parseInt(255 * angle2 / 90), 30, 255)
                left = clamp(parseInt(255 * (2 - angle2 / 90)), 30, 255)
                // console.log("%f , %f" ,left, right)
              }
            }
            var pwn1 = right
            var pwn2 = left
            this.cmessage = [36, 1]
            this.cmessage.push(status_byte)
            this.cmessage.push(pwn1)
            this.cmessage.push(pwn2)
            this.dataList.push("control msg sent: " + this.cmessage);
            this.monitor_socket.send(String.fromCharCode.apply(null, this.cmessage))
          }

        },
        50
    )

    ctx.fillStyle = "#717171";
    ctx.fillRect(this.x_y_loc[0] - 4, this.x_y_loc[1] - 4, 8, 8,)
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
    bootState: function () {
      for (let i = 0; i < 8; i++) {
        this.generateAlian()
      }
    }
  },
  methods: {
    initWebSocket() {
      const gateway = "ws://192.168.4.1:80/control";
      console.log('Trying to open a WebSocket connection...');
      this.monitor_socket = new WebSocket(gateway);
      this.monitor_socket.onopen = this.RegMonitor;
      this.monitor_socket.onclose = this.onClose;
      this.monitor_socket.onmessage = this.onMessage;

    },
    RegMonitor() {
      console.log('monitor socket opening');
      this.monitor_socket.send(String.fromCharCode.apply(null, [36, 0, 0]))
      this.RegGamepad()
      console.log('monitor socket opened');

    },
    RegGamepad() {
      this.gamepad_socket.send(String.fromCharCode.apply(null, [36, 0, 1]))
      console.log('gamepad socket opened');
    },
    RegControl() {
      console.log("control socket opened")
    },
    onClose() {
      console.log('Connection closed');
      setTimeout(this.initWebSocket, 2000);
    },
    onMessage(event) {
      // console.log(event.data.toString())
      this.dataList.push(event.data.toString())
      this.decodeMessage(event.data.toString())
    },
    decodeMessage(msg) {
      var msg_array = msg.split("real:")[1].split(" yaw:")[0].split(" ");
      this.x_y_loc[0] = parseFloat(msg_array[0]) * 2 + 400;
      this.x_y_loc[1] = parseFloat(msg_array[1]) * 2 + 250;
      console.log("x: %f y: %f yaw: ", this.x_y_loc[0], this.x_y_loc[1]);
    },
    pushList() {
      const len = this.dataList.length
      return [{id: 1, text: this.dataList[len - 4]},
        {id: 2, text: this.dataList[len - 3]},
        {id: 3, text: this.dataList[len - 2]},
        {id: 4, text: this.dataList[len - 1]}]
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
      this.mouse_state = true;
    },
    mouseMove(e) {
      if (this.mouse_state && this.driveState) {
        let x = e.offsetX
        let y = e.offsetY
        this.joystick_x = x - 75
        this.joystick_y = y - 100
        this.moveX = this.joystick_x / 50
        this.moveY = this.joystick_y / 50
        console.log("touch moved %d %d", this.joystick_x, this.joystick_y)
      }
    },
    mouseEnd() {
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
        this.moveX = (this.joystick_x - 50) / 50
        this.moveY = (this.joystick_y - 50) / 50
      }
      // console.log("touch moved %d %d",this.moveX,this.moveY)
    },
  }

}
</script>

<style scoped>
.text-container {
  font-size: 14px;
  color: #afafaf;
  /*margin-left: 5%;*/
  /*margin-top: 40px;*/
  height: 150px;
  width: 400px;
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
