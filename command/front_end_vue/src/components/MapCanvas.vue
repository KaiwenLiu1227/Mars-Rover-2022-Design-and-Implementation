<template>
  <div>
    <v-stage ref="stage" :config="configKonva">
      <v-layer ref="layer">
        <v-line :config="configLine" :key="componentKey"/>
      </v-layer>
      <v-layer ref="rover"  :config="configImgLayer">
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
      componentKey: 0,
      configKonva: {
        width: 720,
        height: 535,
        draggable: true
      },
      configCircle: {
        x: 100,
        y: 100,
        radius: 70,
        fill: 'red',
        stroke: 'black',
        strokeWidth: 4
      },
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
      configImg: null
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


      if ((Math.abs(xy_pos[0]) >= (this.configKonva.width / 2 - 30)) || (Math.abs(xy_pos[1]) >= (this.configKonva.height / 2 - 30))) {
        config.scaleX = 0.5
        config.scaleY = 0.5
        this.configImgLayer.scaleX = 0.5
        this.configImgLayer.scaleY = 0.5
        this.configImgLayer.x = 360 + xy_pos[0] / 2
        this.configImgLayer.y = 267 + xy_pos[1] / 2
      } else {
        this.configImgLayer.x = 360 + xy_pos[0]
        this.configImgLayer.y = 267 + xy_pos[1]
      }
      this.configLine = config
      this.componentKey = this.componentKey + 1
      // console.log(config)
      var node = this.$refs.rover.getNode()
      // node.x = this.configImgLayer.x + 100
      // node.offsetY = 20
      node.rotation(this.yaw);
    },
  }
}
</script>

<style scoped>

</style>