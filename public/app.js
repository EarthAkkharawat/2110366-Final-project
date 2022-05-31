// Complete Project Details at: https://RandomNerdTutorials.com/
// import { getDatabase, ref, set } from "firebase/database";
// import { initializeApp } from "https://www.gstatic.com/firebasejs/9.8.2/firebase-app.js";
// import { getAnalytics } from "https://www.gstatic.com/firebasejs/9.8.2/firebase-analytics.js";
// var firebaseConfig = {
//   apiKey: "AIzaSyDU3ZLPqguNfSx2TBqNUsPZnsruVY_H3tQ",
//   authDomain: "smartest-light.firebaseapp.com",
//   databaseURL: "https://smartest-light-default-rtdb.firebaseio.com",
//   projectId: "smartest-light",
//   storageBucket: "smartest-light.appspot.com",
//   messagingSenderId: "863791815861",
//   appId: "1:863791815861:web:cc08cc550c2010b88772fc",
//   measurementId: "G-3WEQ651SPY",
// };
// Initialize Firebase
// firebase.initializeApp(firebaseConfig);

// import {
//   getDatabase,
//   ref,
//   set,
// } from "https://www.gstatic.com/firebasejs/9.1.0/firebase-database.js";

var database = firebase.database();
// const app = initializeApp(firebaseConfig);
// const analytics = getAnalytics(app);

// Database Paths
var dataFloatPath = "test/brightness";
var dataIntPath = "test/buttonState";
var turnOFFPath = "test/turnOFF";
var check = 0;
// const db = getFirestore();
// Get a database reference
const databaseFloat = database.ref(dataFloatPath);
const databaseInt = database.ref(dataIntPath);
const turnOFFInt = database.ref(turnOFFPath);

// Variables to save database current values
var floatReading;
var intReading;

// const db = getDatabase();
// set(ref(db, "test/"), {
//   buttonState: 0,
// });

// Attach an asynchronous callback to read the data
databaseFloat.on(
  "value",
  (snapshot) => {
    floatReading = snapshot.val();
    console.log(floatReading);
    document.getElementById("brightness").innerHTML = floatReading;
    if (floatReading <= 25) {
      document.getElementById("sicon").src = "image/moon.png";
    } else {
      document.getElementById("sicon").src = "image/sun.png";
    }
  },
  (errorObject) => {
    console.log("The read failed: " + errorObject.name);
  }
);

databaseInt.on(
  "value",
  (snapshot) => {
    intReading = snapshot.val();
    console.log(intReading);
    if (intReading == 1) {
      document.getElementById("on-off").innerHTML = "ON";
      document.getElementById("on-off").src = "image/light-bulb.png";
    } else if (intReading == 0) {
      document.getElementById("on-off").innerHTML = "OFF";
      document.getElementById("on-off").src = "image/nolightbut.png";
    }
  },
  (errorObject) => {
    console.log("The read failed: " + errorObject.name);
  }
);

document.getElementById("on-off").addEventListener("click", function onClick() {
  // const db = getDatabase();
  // set(ref(db, 'test/'), {
  //   turnOFF : 1
  // });
  // console.log("test");
  // document.getElementById("on-off").innerHTML = "OFF";
  if (check == 0) {
    console.log("test");
    document.getElementById("imgg").src = "image/nolightbut.png";
    check = 1;

    // const db = getDatabase();
    // set(ref(db, "test/"), {
    //   buttonState: 0,
    // });
    // console.log("test");
  } else {
    console.log("test2");

    document.getElementById("imgg").src = "image/light-bulb.png";
    check = 0;

    // const db = getDatabase();
    // set(ref(db, "test/"), {
    //   buttonState: 1,
    // });
    console.log("test");
    // document.getElementById("on-off").innerHTML = "OFF";
  }
  // btn.style.color = 'white';

  // index = index >= colors.length - 1 ? 0 : index + 1;
});

// function click(){
//   console.log("test");
// }
