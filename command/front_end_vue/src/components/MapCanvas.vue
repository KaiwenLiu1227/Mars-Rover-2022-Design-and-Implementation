<template>
  <div>
    <el-row style="margin-top: 10px">
      <el-col :span="6"></el-col>
      <el-col :span="6">
        <el-switch active-text="ZoomOut" v-model="isZoom" @change="showHandler()"
                   style=""/>
      </el-col>
      <el-col :span="6">
        <el-switch active-text="RadarPlot" v-model="showRadar" @change="showHandler()"
                   style=""/>
      </el-col>
      <el-col :span="6">
        <el-switch active-text="RoutePlot" v-model="showRoute" @change="showHandler()"
                   style=""/>
      </el-col>
    </el-row>
    <v-stage ref="stage" :config="configKonva">
      <v-layer ref="layer" :config="configMainLayer">
        <v-circle v-for="(item,i) in configCircle" :config="configCircle[i]"></v-circle>
        <v-text v-for="(item,i) in configText" :config="configText[i]"></v-text>
        <v-line :config="configLine" :key="componentKey"/>
      </v-layer>
      <v-layer :config="configDotLayer">
        <v-circle v-for="(item,i) in configDots" :config="configDots[i]"></v-circle>
      </v-layer>
      <v-layer ref="rover" :config="configImgLayer">
        <v-image :config="{image: configImg}"/>
      </v-layer>
    </v-stage>

  </div>

</template>

<script>

// import Konva from "konva";

export default {
  name: "MapCanvas",
  props: {
    yaw: {type: Number, default: 0}
  },
  data() {
    return {
      renderComponent: true,
      showRadar: true,
      isZoom:false,
      showRoute: true,
      showCar: true,
      componentKey: 0,
      configKonva: {
        width: 720,
        height: 535,
        scale: 1,
        draggable: true
      },
      configDotLayer:{
        opacity:1
      },
      configMainLayer:{},
      configDots: [{
        x: 100,
        y: 100,
        radius: 2,
        fill: "black"
      }],
      configCircle: [],
      configLine: {
        x: 360,
        y: 267,
        points: [0, 0],
        strokeWidth: 4,
        // tension: 0.1,
        stroke: '#007fb4',
        scaleX: 1,
        scaleY: 1
      },
      configImgLayer: {
        x: 360,
        y: 267,
        offset: {x: 20, y: 20}
      },
      configText:[],
      configImg: null,
      colorList:{9:"white",0:"black",18:"red",27:"blue",36:"yellow",45:"purple",54:"green",63:"cyan"},
      obsList:[]
    }
  },
  mounted() {
  },
  created() {
    const image = new window.Image();
    image.src = "assets/rover_map.png";
    image.height = 40;
    image.width = 40;
    // image.y=267;
    image.onload = () => {
      // set image only when it is loaded
      this.configImg = image;
    };
  },
  methods: {
    addPos(xy_pos) {
      // console.log(xy_pos)
      var config = this.configLine
      config.points.push(xy_pos[0])
      config.points.push(xy_pos[1])
      // var scale = 0.75
      // if ((Math.abs(xy_pos[0]) >= (this.configKonva.width / 2 - 30)) || (Math.abs(xy_pos[1]) >= (this.configKonva.height / 2 - 30))) {
      //   this.configDotLayer.scaleX = scale
      //   this.configDotLayer.scaleY = scale
      //   this.configMainLayer.scaleX = scale
      //   this.configMainLayer.scaleY = scale
      //   this.configImgLayer.scaleX = scale
      //   this.configImgLayer.scaleY = scale
      //   this.configImgLayer.x = 360 + xy_pos[0] * scale
      //   this.configImgLayer.y = 267 + xy_pos[1] * scale
      // } else {
        this.configImgLayer.x = 360 + xy_pos[0]
        this.configImgLayer.y = 267 + xy_pos[1]
      // }
      this.configLine = config
      this.componentKey = this.componentKey + 1
      // console.log(config)
      var node = this.$refs.rover.getNode()
      // node.x = this.configImgLayer.x + 100
      // node.offsetY = 20
      node.rotation((this.yaw/Math.PI)*180);
    },
    addObs(obs){
      // if(parseInt(obs[4])!==18 && parseInt(obs[4])!==27){
      //   return
      // }
      console.log(obs)
      var obsConfig={
        x: -parseFloat(obs[2])+360,
        y: parseFloat(obs[3])+267,
        radius: 8,
        fill: this.colorList[parseInt(obs[4])]
      }
      var textConfig={
        x: -parseFloat(obs[2])+355,
        y: parseFloat(obs[3])+275,
        text: (parseInt(obs[2])/2).toString() + ", " + (parseInt(obs[3])/2).toString()
      }
      console.log(obs)
      if (parseInt(obs[0]) !== -1){
        // console.log(obs)

        if (parseInt(obs[1]) === 1){
          for (var obs_id in this.configCircle){
              var obs_item=this.configCircle[obs_id]
            if (obs_item["fill"] === this.colorList[parseInt(obs[4])]){
              return
            }
          }
          this.configCircle.push(obsConfig)
          this.configText.push(textConfig)
          console.log("new alien")
        }else{
          this.configCircle[parseInt(obs[0])] = obsConfig
          this.configText[parseInt(obs[0])] = textConfig
          console.log("update alien")
        }
        // console.log(this.configCircle)
      }

      //console.log(this.configCircle)
    },
    addLidarDot(dis){
      var dotConfig={
        x: Math.sin(this.yaw) * dis/3 + this.configImgLayer.x,
        y: -Math.cos(this.yaw) * dis/3 + this.configImgLayer.y,
        radius: 2,
        fill: "grey"
      }
      this.configDots.push(dotConfig)
      // console.log(this.configDots)
    },
    showHandler(){
      if(this.showRadar){
        this.configDotLayer.opacity=1
      }else if(!this.showRadar){
        this.configDotLayer.opacity=0
      }
      if(this.isZoom){
        this.configKonva.scaleX = 0.75
        this.configKonva.scaleY = 0.75
      }else if(!this.isZoom){
        this.configKonva.scaleX = 1
        this.configKonva.scaleY = 1
      }
      if(this.showRoute){
        this.configLine.opacity=1
      }else if(!this.showRoute){
        this.configLine.opacity=0
      }
    },
    resetCanvas() {
      this.configLine.scaleX = 1
      this.configLine.scaleY = 1
      this.configLine.points = []
      this.configCircle = []
      this.configDots = []
      this.configText = []
      this.configImgLayer.scaleX = 1
      this.configImgLayer.scaleY = 1
      this.configDotLayer.scaleX = 1
      this.configDotLayer.scaleY = 1
      this.configMainLayer.scaleX = 1
      this.configMainLayer.scaleY = 1
      this.configImgLayer.x = 360
      this.configImgLayer.y = 267
    }
  }
}
</script>

<style scoped>

</style>