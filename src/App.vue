<template>

  <div id="app" >
    <el-container v-show="!is_mobile" style="background-color: white;width: 1350px;margin: auto;" >
      <el-header style="height: 250px">
        <h3 style="position: absolute; margin-left: 450px;width: 400px;margin-top: 120px;">M &nbsp;A &nbsp;R &nbsp;S &nbsp; &nbsp;R
          &nbsp;O &nbsp;V &nbsp;E &nbsp;R</h3>
        <h6 style="color: #a8b2bb; position: absolute; margin-left: 700px;width: 300px;margin-top: 130px; ">ver : 25. 4. 6. 13</h6>
        <el-button round size="small" style="position:absolute; margin-left: 250px;margin-top: 120px;" @click="changeDisplay">Desktop</el-button>
        <particles style="margin-bottom: 10px; margin-top: 10px"/>
      </el-header>
        <el-main><RoverMap/></el-main>
        <el-footer style="width: 80%; margin-top: 100px; margin-bottom: 70px">
          <div style="width: 100px; height: 50px; margin-top: -50px; margin-left: 2.5%; ">
          <img alt="" src="./assets/rover.gif" style="object-fit: scale-down; height: 100px;width: 200px">
          </div>
          <div style="position:absolute; width: 1250px; height: 2px; background-color: #a8b2bb; margin-top: 28px; margin-left: 20px"></div>
          <h5 style="margin-top: 60px;color: #a8b2bb;margin-left: 300px"> Copyright @ all right reserved by group 25 (#｀皿´) </h5>
          <h5 style="margin-top: 10px;color: #a8b2bb;margin-left: 300px"> 沪 ICP备什么号 </h5>
        </el-footer>
    </el-container>

    <el-container v-show="is_mobile" style="background-color: white;margin: auto;" >
      <el-header style="height: 100px;">
        <h3 style="width: 60%;margin-top: 10%;">M &nbsp;A &nbsp;R &nbsp;S &nbsp; &nbsp;R
          &nbsp;O &nbsp;V &nbsp;E &nbsp;R</h3>
        <h6 style="color: #a8b2bb; position: absolute; margin-left: 420px;width: 200px;margin-top: 25px; ">ver : 25. 4. 6. 13</h6>
        <el-button round size="small" style="position:absolute; margin-left: 250px;margin-top: 20px;" @click="changeDisplay">mobile</el-button>
      </el-header>
      <el-main><RoverMap/></el-main>
      <el-footer style="width: 80%; margin-top: 100px; margin-bottom: 70px">
        <div style="width: 100px; height: 50px; margin-top: -50px; margin-left: 2.5%; ">
          <img alt="" src="./assets/rover.gif" style="object-fit: scale-down; height: 100px;width: 200px">
        </div>
        <div style="position:absolute; width: 1250px; height: 2px; background-color: #a8b2bb; margin-top: 28px; margin-left: 20px"></div>
        <h5 style="margin-top: 60px;color: #a8b2bb;margin-left: 300px"> Copyright @ all right reserved by group 25 (#｀皿´) </h5>
        <h5 style="margin-top: 10px;color: #a8b2bb;margin-left: 300px"> 沪 ICP备什么号 </h5>
      </el-footer>
    </el-container>
  </div>


</template>

<script>
import RoverMap from './components/RoverMap.vue'
import particles from "./components/particles.vue"
// Vue.use(VueParticles)

export default {
  name: 'App',
  components: {
    RoverMap,
    particles
  },
  data() {
    return{
      is_mobile: false
    }
  },
  methods: {
    changeDisplay() {
      this.is_mobile=!this.is_mobile
    }
  }
}
</script>

<style>
#app {
  font-family: Avenir, Helvetica, Arial, sans-serif;
  -webkit-font-smoothing: antialiased;
  -moz-osx-font-smoothing: grayscale;
  text-align: center;
  height: 100%;
  /*color: #2c3e50;*/
  background-color: #e2e2e2;
  /*margin-top: 60px;*/
}

</style>
