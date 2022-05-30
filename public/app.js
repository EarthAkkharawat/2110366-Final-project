// Complete Project Details at: https://RandomNerdTutorials.com/
import { getDatabase, ref, set } from "firebase/database";

// Database Paths
var dataFloatPath = "test/brightness";
var dataIntPath = "test/buttonState";
var turnOFFPath = "test/turnOFF";

// Get a database reference
const databaseFloat = database.ref(dataFloatPath);
const databaseInt = database.ref(dataIntPath);
const turnOFFInt = database.ref(turnOFFPath);

// Variables to save database current values
var floatReading;
var intReading;

// Attach an asynchronous callback to read the data
databaseFloat.on(
  "value",
  (snapshot) => {
    floatReading = snapshot.val();
    console.log(floatReading);
    document.getElementsByClassName("brightness").innerHTML = floatReading;
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
    if (intReading == 1){
      document.getElementById("on-off").innerHTML = "ON";
      document.getElementById("on-off").style.backgroundColor = rgb(255, 230, 2);
    }else if (intReading == 0){
      document.getElementById("on-off").innerHTML = "OFF";
      document.getElementById("on-off").style.backgroundColor = rgb(175, 174, 168);
    }
  },
  (errorObject) => {
    console.log("The read failed: " + errorObject.name);
  }
);

document.getElementById("on-off").addEventListener('click', function onClick() {
  const db = getDatabase();
  set(ref(db, 'test/'), {
    turnOFF : 1
  });
  document.getElementById("on-off").innerHTML = "OFF";
  document.getElementById("on-off").style.backgroundColor = rgb(255, 230, 2);
  // btn.style.color = 'white';

  // index = index >= colors.length - 1 ? 0 : index + 1;
});
