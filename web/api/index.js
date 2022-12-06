// load express
const express = require('express');
const app = express();
const { exec } = require('child_process');

app.use(express.json());
//GET /?color1=red&color2=blue

app.get("/", (req, res) => {
  const { filename } = req.query;
  res.header("Access-Control-Allow-Origin", "*");
  console.log("FILE : " + filename);

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

  //Create array of 81 integers
  const array = [];
  for (let i = 1; i < 82; i++) {
    array.push(i);
  }

  let result = {
    array: array
  };
  let r = JSON.stringify(result);
  console.log(r);
  res.send(r);
  res.end();
});


app.listen(3001, () => { 
    console.log('Started on port 3001');
});

//Path API: http://localhost:3000/upload

//Key: file, Value: [name].jpeg
//Key: description, Value: [description]