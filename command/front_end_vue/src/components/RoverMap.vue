<template>
  <div>
    <el-container @mousemove="mouseMove"
                  @mouseup="mouseEnd">
      <el-main width="400px" style="margin-left: 10px">
        <div
            class="shadedbox" style="height: 220px;margin-bottom: 40px;">
          <el-col style="margin-left: 15px;margin-top: 15px">
            <el-row>
              <el-col :span="7">
                <el-row>
                  <el-switch active-text="Pause" v-model="pauseState" @change="pauseHandler" :disabled="!connectState"/>
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
                        {{ (parseInt(180 * yaw / Math.PI)) }}</h6></div>
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
                  <el-switch :active-text="ModeDisplay" v-model="driveState"
                             @change="modeSwitchHandler" :disabled="!connectState"/>
                  <!--                  <el-switch v-if="!connectState" disabled :active-text="ModeDisplay"/>-->
                </el-row>
                <el-row style="margin-top: 5px">
                  <el-col :span="8">
                    <h6>Sensor. Quality.</h6>
                  </el-col>
                  <el-col :span="4" style="margin-top: 3px">
                    <el-progress type="dashboard" :color="colors" :percentage="sensor_quality" :width=70></el-progress>
                  </el-col>
                </el-row>
              </el-col>
            </el-row>
            <el-row>
              <el-col :span="14"
                      style="background-color: rgba(255,255,255,0.18); border-radius: 10px;padding-top: 10px;margin-top: -5px;padding-bottom: 5px;padding-left: 5px;margin-left: -5px">
                <el-row>
                  <div class="text-container" style="overflow: hidden;word-break: break-all">
                    <p class="text" v-for="value in pushList()" :key="value.id">{{ value.text }}</p>
                  </div>
                </el-row>
                <el-row>
                  <input
                      style="width: 200px;height: 20px;"
                      placeholder="Enter command line"
                      v-model="cmd_input"
                      @keyup.enter="enterCMD()"
                  >
                </el-row>
              </el-col>
              <el-col :span="2" style="margin-left: 30px; margin-right: 11px;margin-top: 10px">
                <h6 style="margin-top: 15px;">Yaw:</h6>
              </el-col>
              <el-col :span="4" style="margin-top: 10px">
                <div
                    style="height: 60px; width: 60px; border-radius: 100%; margin-top: -10px;margin-left: 10px;"
                    class="shadedbox">
                  <img v-if="connectState" alt="" src="../assets/arrow_on.png"
                       style="object-fit: scale-down;  height: 50px;width: 50px;margin-top: 5px"
                       v-bind:style="{transform: 'rotate('+ ((yaw*180)/Math.PI) +'deg)'}">
                  <img v-if="!connectState" alt="" src="../assets/arrow.png"
                       style="object-fit: scale-down; opacity: 0.6; height: 50px;width: 50px;margin-top: 5px">
                </div>
              </el-col>
            </el-row>
          </el-col>
        </div>
        <div
            class="shadedbox" style="height: 270px; ">
          <el-row style="margin-left: 10px;margin-top: 10px">
            <el-col :span="4" style="margin-right: 5px">
              <el-row>
                <el-button class="ctrl_button" :disabled="!connectState" @click="NavigateStart()">Start</el-button>
              </el-row>
              <el-row>
                <el-button class="ctrl_button" :disabled="!connectState" @click="ResetAll()">Reset</el-button>
              </el-row>
              <el-row>
                <el-button class="ctrl_button" :disabled="!connectState" @click="ResetCanvas()">Clear</el-button>
              </el-row>
            </el-col>
            <el-col :span="2" style="margin-right: 15px">
              <div
                  class="shadedbox" style="width: 30px;">
                <el-slider
                    :disabled="!driveState"
                    :min="-100"
                    :max="100"
                    v-model="slideX"
                    vertical
                    height="180px"
                    style="margin-left: -4px;margin-bottom: 16px;margin-top: 16px;"
                >
                </el-slider>
              </div>
              <h6 style="margin-top: 8px; color: #797979">Speed X</h6>
            </el-col>
            <el-col :span="2" style="margin-right: 15px">
              <div
                  class="shadedbox" style="width: 30px;">
                <el-slider
                    :disabled="!driveState"
                    v-model="slideY"
                    :min="-100"
                    :max="100"
                    vertical
                    height="180px"
                    style="margin-left: -4px;margin-bottom: 16px;margin-top: 16px;"
                >
                </el-slider>
              </div>
              <h6 style="margin-top: 8px; color: #797979">Speed Y</h6>
            </el-col>
            <el-col :span="2" style="margin-right: 15px">
              <div
                  class="shadedbox" style="width: 30px;">
                <el-slider
                    disabled
                    v-model="intensity"
                    vertical
                    height="180px"
                    style="margin-left: -4px;margin-bottom: 16px;margin-top: 16px;"
                >
                </el-slider>
              </div>
              <h6 style="margin-top: 8px; color: #797979">Motion Intensity</h6>
            </el-col>
            <el-col :span="6">
              <div class="joystick" id="gamepad"
                   style="  width: 200px; height: 200px; margin-left: 0px;margin-top: 15px"
                   @touchstart="touchstart"
                   @touchmove="touchmove"
                   @touchend="touchend"
                   @mousedown="mouseStart"
              >
                <div v-if="driveState" class="joystick"
                     style="width: 100px; height: 100px; position: relative; background-color: #5ba3e7"
                     v-bind:style="{left: joystick_x+'px', top: joystick_y+'px'}"
                ></div>
                <div v-if="!driveState" class="joystick"
                     style="width: 100px; height: 100px; position: relative; background-color: #a8b2bb; top:50px;left: 50px"
                ></div>
              </div>
            </el-col>
          </el-row>
        </div>
      </el-main>
      <el-main style="margin: auto;">
        <!--        <canvas class="canvas" :width="720" :height="535" id="ctx"-->
        <!--        ></canvas>-->
        <MapCanvas class="shadedbox" ref="map_canvas" :yaw="yaw"></MapCanvas>
      </el-main>
    </el-container>
  </div>
</template>

<script>
import MapCanvas from "@/components/MapCanvas";

export default {
  name: 'RoverMap',
  props: {
    msg: String,
  },
  components: {
    MapCanvas
  },
  data() {
    return {
      web_socket: null,
      pauseState: false,
      resetState: false,
      connectState: false,
      driveState: false,
      mouse_state: false,
      hasAlert: false,
      nav_state: false,
      intensity: 0,
      sensor_quality: 0,
      cmd_input: '',
      slideX: 0,
      slideY: 0,
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
      ModeDisplay: "Auto Mode",
      colors: [
        {color: '#f56c6c', sensor_quality: 10},
        {color: '#e6a23c', sensor_quality: 20},
        {color: '#5cb87a', sensor_quality: 30},
        {color: '#1989fa', sensor_quality: 40},
        {color: '#1989fa', sensor_quality: 50}
      ],
      debug_yaw: 0
    }
  },
  mounted() {
    let that = this
    setInterval(
        () => {
          //gamepad test comment following part for real situation
          // this.x_y_loc[0] += this.moveX /10
          // this.x_y_loc[1] += this.moveY /10

          //monitor
          // this.xy_str = [-this.x_y_loc[0] + 350, -this.x_y_loc[1] + 250].toString()
          // this.pushList().push(this.xy_str)

          //gamepad
          if (this.connectState && this.driveState) {
            // this.sendAlert("Started control", "success")
            var x2 = 0
            var y2 = 0
            if ((this.moveX !== 0) && (this.moveY !== 0)) {
              x2 = -this.moveX
              y2 = -this.moveY
            } else {
              x2 = this.slideX
              y2 = this.slideY
            }
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
              if (mag2 <= 5) {
                left = 0
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
            var pwn1 = parseInt(left / 2)
            var pwn2 = parseInt(right / 2)
            // console.log("%f , %f" ,pwn1, pwn2)
            var cmessage = new Uint8Array([36, 1, status_byte, pwn1, pwn2])
            this.dataList.push("control msg sent: " + cmessage);
            // console.log(cmessage)
            // var string=""
            // for(var i=0;i<cmessage.length;i++){
            //   string+=String.fromCharCode(cmessage[i])
            // }
            // if(this.web_socket.isReady){
            //   this.sendAlert("Control message sent", "success")
            //   this.web_socket.send(String.fromCharCode.apply(null, cmessage))
            // }
            this.web_socket.send(String.fromCharCode.apply(null, cmessage))
          }
          this.debug_yaw += 10
          if (that.$refs.map_canvas) {
            that.$refs.map_canvas.addPos(this.x_y_loc);
          }

        },
        150
    )
  },
  watch: {
    // xy_str: function (new_XY, old_XY) {
    //   const canvas = document.getElementById("ctx");
    //   const ctx = canvas.getContext("2d");
    //   // ctx.translate(360,278)
    //   const newXY = [parseFloat(new_XY.split(",")[0]), parseFloat(new_XY.split(",")[1])]
    //   const oldXY = [parseFloat(old_XY.split(",")[0]), parseFloat(old_XY.split(",")[1])]
    //
    //   // erase original arrow
    //   //eraser 1
    //   // ctx.moveTo(oldXY[0] + 32 * Math.cos(this.yaw), oldXY[1] - 32 * Math.sin(this.yaw))
    //   // ctx.lineTo(oldXY[0] + 10 * Math.sin(this.yaw) - 20 * Math.cos(this.yaw), oldXY[1] - 10 * Math.cos(this.yaw) + 20 * Math.sin(this.yaw))
    //   // ctx.lineTo(oldXY[0] + 2 * Math.cos(this.yaw), oldXY[1] - 2 * Math.sin(this.yaw))
    //   // ctx.lineTo(oldXY[0] - 10 * Math.sin(this.yaw) + 20 * Math.cos(this.yaw), oldXY[1] + 10 * Math.cos(this.yaw) - 20 * Math.sin(this.yaw))
    //   // ctx.fillStyle = "#ffffff";
    //   // ctx.fill()
    //
    //   //eraser 2
    //   ctx.fillStyle = "#ffffff";
    //   ctx.fillRect(oldXY[0] - 5, oldXY[1] - 5, 10, 10);
    //
    //   ctx.fillStyle = 'red'
    //   ctx.fillRect(-10, -10, 20,20)
    //
    //   ctx.strokeStyle = '#3350e0';
    //
    //   //draw line
    //
    //   ctx.lineWidth = 1;
    //   ctx.moveTo(oldXY[0], oldXY[1]);
    //   ctx.lineTo(newXY[0], newXY[1]);
    //
    //   //draw arrow
    //   // ctx.strokeStyle = '#c70303';
    //   // ctx.beginPath()
    //   // ctx.moveTo(newXY[0] + 20 * Math.cos(this.yaw), newXY[1] - 20 * Math.sin(this.yaw))
    //   // ctx.lineTo(newXY[0] + 2 * Math.sin(this.yaw), newXY[1] + 2 * Math.cos(this.yaw))
    //   // ctx.lineTo(newXY[0] - 2 * Math.sin(this.yaw), newXY[1] - 2 * Math.cos(this.yaw))
    //   // ctx.lineTo(newXY[0] + 20 * Math.cos(this.yaw), newXY[1] - 20 * Math.sin(this.yaw))
    //
    //   //draw square
    //   ctx.fillStyle = "#6b45a1";
    //   ctx.fillRect(newXY[0] - 3, newXY[1] - 3, 6, 6);
    //   // ctx.save()
    //   ctx.scale(0.5,0.5)
    //   ctx.stroke();
    //
    //   // this.dataList.push("xy: " + parseInt(newXY[0]) + " " + parseInt(newXY[1]));
    //   // this.dataList.push("yaw: " + this.yaw)
    // },
  },
  methods: {
    ResetAll() {
      this.x_y_loc = [0, 0];
      this.yaw = 0;
      this.sensor_quality = 0;
      this.intensity = 0;
      this.slideX = 0;
      this.slideY = 0;
      var cmessage = new Uint8Array([36, 2, 5])
      this.dataList.push("control msg sent: reset");
      this.web_socket.send(String.fromCharCode.apply(null, cmessage))
      this.ResetCanvas()
    },
    ResetCanvas() {
      this.$refs.map_canvas.resetCanvas();
    },

    initWebSocket() {
      const gateway = "ws://192.168.1.80:80/control";
      console.log('Trying to open a WebSocket connection...');
      this.sendAlert("Trying to open a WebSocket connection...", "info")
      this.web_socket = new WebSocket(gateway);
      this.web_socket.binaryType = "arraybuffer"
      this.web_socket.onopen = this.RegMonitor;
      this.web_socket.onclose = this.onClose;
      this.web_socket.onmessage = this.onMessage;
    },
    RegMonitor() {
      console.log('monitor socket opening');
      this.sendAlert("Monitor socket opening", "info")
      this.connectState = true
      this.web_socket.send(String.fromCharCode.apply(null, [36, 0, 0]))
      // this.RegGamepad()
      console.log('monitor socket opened');
      this.sendAlert("Monitor socket opened", "success")

    },
    NavigateStart() {
      var state = 0;
      var pos_x = 0;
      var pos_y = 0;
      // console.log("navigate")
      this.sendAlert("Starting navigation", "info")
      let that = this
      this.nav_state = setInterval(function () {
        if (state === 0) {
          pos_y = 200;
        } else if (state === 1) {
          pos_x = pos_x + 100
        } else if (state === 2) {
          pos_y = 50
        } else if (state === 3) {
          state = -1
          pos_x = pos_x + 100
        }
        state = state + 1
        var cmessage = [36]
        cmessage.push(2)
        cmessage.push(1)
        if (pos_x > 0) {
          cmessage.push(1)
        } else {
          cmessage.push(0)
        }
        cmessage.push(Math.abs(pos_x))
        if (pos_y > 0) {
          cmessage.push(1)
        } else {
          cmessage.push(0)
        }
        cmessage.push(Math.abs(pos_y))
        // that.x_y_loc = [pos_x, pos_y]
        // that.xy_str = [-that.x_y_loc[0] + 700, -that.x_y_loc[1] + 500].toString()
        // console.log(that.xy_str)
        that.pushList().push(cmessage)
        that.web_socket.send(String.fromCharCode.apply(null, cmessage))
      }, 500);
    },
    pauseHandler() {
      if (this.pauseState) {
        this.web_socket.send(String.fromCharCode.apply(null, [36, 2, 3, 3]))
        clearInterval(this.nav_state)
      } else {
        if (this.driveState) {
          this.web_socket.send(String.fromCharCode.apply(null, [36, 2, 3, 2]))
        } else {
          this.web_socket.send(String.fromCharCode.apply(null, [36, 2, 3, 0]))
        }
        // this.NavigateStart()
      }
    },

    enterCMD: function () {
      // console.log("enter")
      if (this.cmd_input.includes("pos")) {
        var pos_x = parseInt(this.cmd_input.split("pos")[1].split(",")[0])
        var pos_y = parseInt(this.cmd_input.split("pos")[1].split(",")[1])
        this.cmd_input = ""
        var cmessage = [36, 2, 1]
        if (pos_x > 0) {
          cmessage.push(1)
        } else {
          cmessage.push(0)
        }
        cmessage.push(Math.abs(pos_x))
        if (pos_y > 0) {
          cmessage.push(1)
        } else {
          cmessage.push(0)
        }
        cmessage.push(Math.abs(pos_y))
        this.dataList.push("control msg sent: " + cmessage);
        if (this.connectState) {
          this.web_socket.send(String.fromCharCode.apply(null, cmessage))
          this.sendAlert("Command sent", "success")
        } else {
          this.sendAlert("No connection!", "warning")
        }
      } else if (this.cmd_input.includes("obs")) {
        var obs = this.cmd_input.split("obs ")[1].split(" ")
        this.$refs.map_canvas.addObs(obs);
        this.sendAlert("Command sent", "success")
      } else {
        this.sendAlert("Invalid input", "warning")
      }
    },
    connectHandler() {
      if (this.connectState) {
        this.initWebSocket()
      } else {
        this.connectState = false
        this.driveState = false
        this.ResetAll()
        this.web_socket.close()
      }
    },
    modeSwitchHandler() {
      if (this.driveState) {
        this.ModeDisplay = "Manual Mode"
        this.web_socket.send(String.fromCharCode.apply(null, [36, 2, 3, 2]))
      } else {
        this.ModeDisplay = "Auto Mode"
        this.web_socket.send(String.fromCharCode.apply(null, [36, 2, 3, 0]))
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
        this.intensity = parseFloat(msg.split("ctrl:")[1].split(",")[0]) / 5.1
        var mpu = parseFloat(msg.split("mpu:")[1].split(",")[0])
        console.log(mpu)
        var mode = parseInt(msg.split("mode:")[1].split(",")[0])
        if (this.driveState) {
          if (mode !== 2) {
            this.sendAlert("Wrong drive mode!", "warning")
          }
        } else {
          if (this.pauseState && (mode === 3)) {
            console.log()
            // this.sendAlert("Mode Switched!", "success")
          } else if (mode === 0) {
            console.log()
            // this.sendAlert("Mode Switched!", "success")
          } else {
            this.sendAlert("Wrong drive mode!", "warning")
          }
        }
        this.$refs.map_canvas.addPos(this.x_y_loc);
      } else if (msg.indexOf("$obs:") !== -1) {
        var obs = msg.split("$obs:")[1].split(" ")
        this.$refs.map_canvas.addObs(obs);
      }
    },
    sendAlert(str, state) {
      let that = this
      if (!this.hasAlert) {
        this.$message({
          message: str,
          type: state
        });
        this.hasAlert = true
        setTimeout(function () {
          that.hasAlert = false
        }, 1000);
      }

    },
    pushList() {
      const len = this.dataList.length
      return [
        {id: 1, text: this.dataList[len - 2]},
        {id: 2, text: this.dataList[len - 1]}]

    },
    mouseStart() {
      // console.log("click down!")
      this.mouse_state = true;
    },
    mouseMove(e) {
      const clamp = (num, min, max) => Math.min(Math.max(num, min), max);
      var rect = document.getElementById('gamepad').getBoundingClientRect()
      var x = e.pageX - rect.x
      var y = e.pageY - rect.y
      if (this.mouse_state & this.driveState) {
        this.joystick_x = clamp(0.5 * (x - this.m_joystick_x_init), -100, 100) + 50
        this.joystick_y = clamp(0.5 * (y - this.m_joystick_y_init), -100, 100) + 50
        this.moveX = (this.joystick_x - 50)
        this.moveY = (this.joystick_y - 50)

        // console.log("touch moved %d %d", x - this.m_joystick_x_init, y - this.m_joystick_y_init)
      } else {
        this.m_joystick_x_init = x
        this.m_joystick_y_init = y
      }

    },
    mouseEnd() {
      // console.log("click up!")
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

.ctrl_button {
  margin-top: 5px;
  margin-bottom: 20px;
  width: 60px;
  height: 50px;
  border: 1px solid rgb(199, 198, 198);
  box-shadow: 2px 2px 2px 1px rgba(74, 74, 255, 0.2);
}
</style>
