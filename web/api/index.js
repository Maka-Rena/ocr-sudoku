// load express
const express = require('express');
const app = express();
const { exec } = require('child_process');
const fs = require("fs");

app.use(express.json());
//GET /?color1=red&color2=blue

function process (filename) {
  // exec(`cd ../ && ls`, (error, stdout, stderr) => {
  //   if (error) {
  //     //console.error(`error: ${error.message}`);
  //     res.send('KO');
  //     return;
  //   }

  //   /*if (stderr) {
  //     console.error(`stderr: ${stderr}`);
  //     return;
  //   }*/

  //   console.log(`stdout:\n${stdout}`);
  // });
}

app.get("/", (req, res) => {
  const { filename, handwritted, hexa } = req.query;
  res.header("Access-Control-Allow-Origin", "*");
  console.log("FILE : " + filename);
  console.log("Is handwritted : " + handwritted);
  console.log("IS Hexa : " + hexa);
  process(filename);
  res.send("OK");
  res.end();
});

app.get("/step", (req, res) => {
  res.header("Access-Control-Allow-Origin", "*");
  const { currentStep } = req.query;
  //Check if file path : ../website/src/components/assets/process/currentStep+1.jpeg exists
  //If yes, send currentStep+1
  //If no, send currentStep
  let step = currentStep;
  if (currentStep < 3 && fs.existsSync("../website/src/components/Upload/process/" + (parseInt(currentStep) + 1) + ".jpeg")) {
    step = parseInt(currentStep) + 1;
  }
  else{
    if (fs.existsSync("../website/src/components/Upload/process/solvedSudoku.json"))
    {
      step = 4;
    }
  }
  console.log("STEP sent: " + step);
  res.send({ step: step });
  res.end();
});

app.get("/result", (req, res) => {
  res.header("Access-Control-Allow-Origin", "*");
  let rawdata = fs.readFileSync('../website/src/components/Upload/process/solvedSudoku.json');
  let result = JSON.parse(rawdata);
  console.log(result["sudoku"]);
  res.send(result["sudoku"]);
  res.end();
});

app.listen(3001, () => { 
    console.log('Started on port 3001');
});

//Path API: http://localhost:3000/upload

//Key: file, Value: [name].jpeg
//Key: description, Value: [description]