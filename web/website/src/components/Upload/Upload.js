import React, { useState } from 'react';
import "./Upload.css";
import upload from "../assets/upload.png";
import axios from 'axios';

const Upload = () => {
    const [go, setGo] = useState(false);
    const [file, setFile] = useState("");
    const [filename, setFilename] = useState("");
    const [progress, setProgress] = useState(0);
    const [step, setStep] = useState(0);
    const [processState, setProcessState] = useState(["⏳ -- Starting process..."]);

    const handleChange = (e) => {
        setFilename(e.target.files[0].name);
        setFile(URL.createObjectURL(e.target.files[0]))
    }
    const [result, setResult] = useState([]);

    const handleSubmit = async () => {
        console.log("FILE : ", filename);
        axios.get("http://localhost:3001/?filename=" + filename, {
            progressEvent: (e) => {
                setProgress(Math.round((e.loaded * 100) / e.total));
            }
        }).then(res => {
                if (res.data === "OK")
                {
                    console.log("Starting process... : ", res.data)
                    setGo(true);
                }
            })
            .catch(err => {
                console.log(err);
            });
    };

    const checkStep = () => {
        axios.get("http://localhost:3001/step?currentStep=" + step)
            .then(res => {
                setStep(res.data.step);
            })
            .catch(err => {
                console.log(err);
            });
        if (step === 1 || step === 0) {
            setProcessState(["⏳ -- Starting process...", "⏳ -- Loading image..."]);
        }
        if (step === 2) {
            setProcessState(["⏳ -- Starting process...", "✅ -- Image loaded", "⏳ -- Processing image..."]);
        }
        if (step === 3) {
            setProcessState(["⏳ -- Starting process...", "✅ -- Image loaded", "✅ -- Image processed", "⏳ -- Solving sudoku..."]);
        }
        if (step === 4) {
            //Get JSON file
            axios.get("http://localhost:3001/result")
                .then(res => {
                    const arr = res.data;
                    let result = [];
                    for (let i = 0; i < 9; i++) {
                        let temp = [];
                        for (let j = 0; j < 9; j++) {
                            temp.push(arr.array[i * 9 + j]);
                        }
                        result.push(temp);
                    }
                    setResult(result);
                })
                .catch(err => {
                    console.log(err);
                });
            setProcessState(["⏳ -- Starting process...", "✅ -- Image loaded", "✅ -- Image processed", "✅ -- Sudoku solved", "✅ -- Done"]);
        }
        console.log("step : ", step);        
    };

    const doubleCheck = () => {
        try {
            return require("./process/" + step.toString() + ".jpeg");
        }
        catch (err) {
            console.log("Err : ", err);
            return require("./loading.gif");
        }
    };

    return (
        <div class="upload-container" id="upload">
            <div class="upload-first-row">
                <p class="upload-title">Upload your picture</p>
                <div class="upload-file-container">
                    <img class="upload-file-icon" width="60" src={upload} alt="upload icon" />
                    <label for="file" class="upload-file-label">{filename !== "" ? filename : "Choose a file"}</label>
                    <input type="file" id="file" accept="image/*" class="upload-input" onChange={handleChange} />
                    {/* eslint-disable-next-line */}
                    <a class="upload-file-submit" onClick={() => handleSubmit()}>Process</a>
                    <div class="upload-progress-container">
                        <div class="upload-progress-bar" style={{ width: progress + "%" }}></div>
                    </div>
                </div>
            </div>
            {go && file !== "" && (
                <>
                    <div class="upload-file-process">
                        <div class="upload-result" style={{ marginRight: 10 }}>
                            <p class="upload-file-process-title">Initial</p>
                            <img class="upload-file-process-image" src={file} alt="preview" />
                        </div>
                        <div class="upload-result"  style={{ marginLeft: 10 }}>
                            <div class="upload-result-right">
                                <p class="upload-file-process-title">Processing...</p>
                                <div class="upload-next-step-container">
                                    <span class="upload-next-step" onClick={() => checkStep()}>
                                        Next step
                                    </span>
                                </div>
                            </div>
                            <div class="result-list-container">
                                {step === 4 && result !== [] && (result.map((item, index) => {
                                    return (
                                        <div class="result-list" key={index}>
                                            {item.map((item2, index2) => {
                                                return (
                                                    <div class="result-list-item" key={index2}>
                                                        <p class="result-list-item-title">{item2}</p>
                                                    </div>
                                                )
                                            }
                                            )}
                                        </div>
                                    )
                                }))}
                            </div>
                            {step !== 4 &&
                                (<img
                                    class="upload-file-process-image"
                                    /* eslint-disable-next-line */
                                    src={doubleCheck()}
                                    alt="preview"
                                />)
                            }
                        </div>
                    </div>
                    <div class="upload-term-container">
                    {processState.map((item, index) => {
                                return (
                                    <div class="upload-term-line" key={index}><span class="term-origin">{" >> ocr-sudoku/$"}</span> {item}</div>
                                )
                            }
                        )}
                    </div>
                </>)}
        </div>
    )
}

export default Upload