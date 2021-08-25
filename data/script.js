let scene, camera, rendered, cube;

function parentWidth(elem) {
  return elem.parentElement.clientWidth;
}

function parentHeight(elem) {
  return elem.parentElement.clientHeight;
}

function init3D(){
  scene = new THREE.Scene();
  scene.background = new THREE.Color(0xffffff);

  camera = new THREE.PerspectiveCamera(75, parentWidth(document.getElementById("3Dcube")) / parentHeight(document.getElementById("3Dcube")), 0.1, 1000);
  console.log("width", parentWidth(document.getElementById("3Dcube")));
  console.log("height", parentHeight(document.getElementById("3Dcube")));
  renderer = new THREE.WebGLRenderer({ antialias: true });
  renderer.setSize(parentWidth(document.getElementById("3Dcube")), 500);

  document.getElementById('3Dcube').appendChild(renderer.domElement);

  
  // Create a geometry
  const geometry = new THREE.BoxGeometry(5, 1, 4);

  // Materials of each face
  var cubeMaterials = [
    new THREE.MeshBasicMaterial({color:0xDFFF00}),
    new THREE.MeshBasicMaterial({color:0xFFBF00}),
    new THREE.MeshBasicMaterial({color:0xFF7F50}),
    new THREE.MeshBasicMaterial({color:0x9FE2BF}),
    new THREE.MeshBasicMaterial({color:0x40E0D0}),
    new THREE.MeshBasicMaterial({color:0xDE3163}),
  ];

  const material = new THREE.MeshFaceMaterial(cubeMaterials);

  cube = new THREE.Mesh(geometry, material);
  
  scene.add(cube);
  camera.position.z = 5;
  renderer.render(scene, camera);


/*
  const boxWidth = 5;
  const boxHeight = 2;
  const boxDepth = 4;
  const geometry = new THREE.BoxGeometry(boxWidth, boxHeight, boxDepth);

  const material = new THREE.MeshBasicMaterial({color: 0x03045e});  // greenish blue

  cube = new THREE.Mesh(geometry, material);
  scene.add(cube);
  camera.position.z = 5;
  renderer.render(scene, camera);
  */
}

// Resize the 3D object when the browser window changes size
function onWindowResize(){
  camera.aspect = parentWidth(document.getElementById("3Dcube")) / parentHeight(document.getElementById("3Dcube"));
  //camera.aspect = window.innerWidth /  window.innerHeight;
  camera.updateProjectionMatrix();
  //renderer.setSize(window.innerWidth, window.innerHeight);
  renderer.setSize(parentWidth(document.getElementById("3Dcube")), parentHeight(document.getElementById("3Dcube")));

}

window.addEventListener('resize', onWindowResize, false);

// Create the 3D representation
init3D();

// Create events for the sensor readings
if (!!window.EventSource) {
  var source = new EventSource('/events');

  source.addEventListener('open', function(e) {
    console.log("Events Connected");
  }, false);

  source.addEventListener('error', function(e) {
    if (e.target.readyState != EventSource.OPEN) {
      console.log("Events Disconnected");
    }
  }, false);

  source.addEventListener('gyro_readings', function(e) {
    //console.log("gyro_readings", e.data);
    var obj = JSON.parse(e.data);
    document.getElementById("gyroX").innerHTML = obj.gyroX;
    document.getElementById("gyroY").innerHTML = obj.gyroY;
    document.getElementById("gyroZ").innerHTML = obj.gyroZ;

    // Change cube rotation after receiving the readings
    //cube.rotation.x = 4 * ((obj.gyroX - 16000) / 32000) -2;
    //cube.rotation.z = 4 * ((obj.gyroZ - 16000) / 32000) -2;
    //cube.rotation.y = 4 * ((obj.gyroY - 16000) / 32000) -2;
    //renderer.render(scene, camera);
  }, false);

  source.addEventListener('temperature_reading', function(e) {
    console.log("temperature_reading", e.data);
    var obj = JSON.parse(e.data);
    document.getElementById("temp").innerHTML = obj.temp;
    document.getElementById("humi").innerHTML = obj.humi;
  }, false);

  source.addEventListener('accelerometer_readings', function(e) {
    console.log("accelerometer_readings", e.data);
    var obj = JSON.parse(e.data);
    document.getElementById("accX").innerHTML = obj.accX;
    document.getElementById("accY").innerHTML = obj.accY;
    document.getElementById("accZ").innerHTML = obj.accZ;

    cube.rotation.x = 4 * ((obj.accX - 16000) / 32000) -2;
    cube.rotation.z = 4 * ((obj.accZ - 16000) / 32000) -2;
    cube.rotation.y = 4 * ((obj.accY - 16000) / 32000) -2;
    renderer.render(scene, camera);

  }, false);
}

function resetPosition(element){
  var xhr = new XMLHttpRequest();
  xhr.open("GET", "/"+element.id, true);
  console.log(element.id);
  xhr.send();
}