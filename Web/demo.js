
// Binding value with click menu

const TEMP = document.getElementById('temp');
const RAIN = document.getElementById('rain');
const HUMD = document.getElementById('humd');


function binding(elementGroup) {
  database.ref(elementGroup).on("value", function (snapshot) {
    TEMP.innerText = snapshot.val()['Temp'] + " °C";
    HUMD.innerText = snapshot.val()['Humd'] + " %";
    RAIN.innerText = snapshot.val()['Rain'] + " mm";
  })
}

// Control device for STATE on firebase

const FAN_PATH = 'Control/Fan';
const LIGHT_PATH = 'Control/Light';
const SOUND_PATH = 'Control/Sound';

function onStateFireBase(elementState) {
  database.ref(elementState).set(1);
}

function offStateFireBase(elementState) {
  database.ref(elementState).set(0);
}

const IMG_FAN_ON = 'img/fanon.png'
const IMG_FAN_OFF = 'img/fanoff.png'
const IMG_SOUND_ON = 'img/soundon.png'
const IMG_SOUND_OFF = 'img/soundoff.png'
const IMG_LED_ON = 'img/ledon.png'
const IMG_LED_OFF = 'img/ledoff.png'

const fan = document.getElementById('fanControl');
const sound = document.getElementById('soundControl');
const light = document.getElementById('lightControl');


const fanCheckBox = fan.querySelector('input');
const soundCheckBox = sound.querySelector('input');
const lightCheckBox = light.querySelector('input');



fanCheckBox.addEventListener('click', function () {
  if (this.checked) {
    fan.querySelector('img').src = IMG_FAN_ON;
    onStateFireBase(FAN_PATH);
  }
  else {
    fan.querySelector('img').src = IMG_FAN_OFF;
    offStateFireBase(FAN_PATH);
  }
})

soundCheckBox.addEventListener('click', function () {
  if (this.checked) {
    sound.querySelector('img').src = IMG_SOUND_ON;
    onStateFireBase(SOUND_PATH);
  }
  else {
    sound.querySelector('img').src = IMG_SOUND_OFF;
    offStateFireBase(SOUND_PATH);
  }
})
lightCheckBox.addEventListener('click', function () {
  if (this.checked) {
    light.querySelector('img').src = IMG_LED_ON;
    onStateFireBase(LIGHT_PATH);
  }
  else {
    light.querySelector('img').src = IMG_LED_OFF;
    offStateFireBase(LIGHT_PATH);
  }
})

//Refesh web to set STATE in firebase ZERO
function loadWeb() {
  offStateFireBase(SOUND_PATH);
  offStateFireBase(LIGHT_PATH);
  offStateFireBase(FAN_PATH);
  binding('RealTime');
}


// Display CLOCK retime
setInterval(function () {
  const clock = document.querySelector(".display");
  let time = new Date();
  let sec = time.getSeconds();
  let min = time.getMinutes();
  let hours = time.getHours();
  let day = 'AM';
  if (hours > 12) {
    day = 'PM';
    hours = hours - 12;
  }
  if (hours == 0) {
    hours = 12;
  }
  if (sec < 10) {
    sec = '0' + sec;
  }
  if (min < 10) {
    min = '0' + min;
  }
  if (hours < 10) {
    hours = '0' + hours;
  }
  clock.textContent = hours + ':' + min + ':' + sec + " " + day;
});