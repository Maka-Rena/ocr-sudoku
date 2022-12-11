import React, { useEffect, useState } from 'react';
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
    const [isHandwrite, setIsHandwrite] = useState(false);
    const [result, setResult] = useState([]);
    const [isHexa, setIsHexa] = useState(false);

    const MAX_STEP = 6;

    const handleChange = (e) => {
        setFilename(e.target.files[0].name);
        setFile(URL.createObjectURL(e.target.files[0]))
    }

    const handleSubmit = async () => {
        if (filename === "")
        {  
            alert("Please select a file");
            return;
        }
        console.log("FILE : ", filename);
        axios.get("http://localhost:3001/?filename=" + filename
            + "&handwritted=" + (isHandwrite === true).toString()
            + "&hexa=" + (isHexa === true).toString(), {
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
                if (res.data.step !== step + 1 && res.data.step !== step)
                    setStep(step+1);
                else{
                    setStep(res.data.step);
                }
                console.log(step);
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
            setProcessState(["⏳ -- Starting process...", "✅ -- Image loaded", "✅ -- Image processed", "✅ -- Sudoku solved", "⏳ -- Done"]);
        }
        if (step === MAX_STEP) {
            //Get JSON file
            axios.get("http://localhost:3001/result")
                .then(res => {
                    const arr = res.data;
                    let result = [];
                    for (let i = 0; i < 9; i++) {
                        let temp = [];
                        for (let j = 0; j < 9; j++) {
                            temp.push(arr[i * 9 + j]);
                        }
                        result.push(temp);
                    }
                    setResult(result);
                })
                .catch(err => {
                    console.log(err);
                });
            console.log("Result : ", result)
            setProcessState(["⏳ -- Starting process...", "✅ -- Image loaded", "✅ -- Image processed", "✅ -- Sudoku solved", "✅ -- Done"]);
        }
        console.log("step : ", step);        
    };

    const doubleCheck = () => {
        try {
            return require("./process/" + step.toString() + ".png");
        }
        catch (err) {
            console.log("Err : ", err);
            return require("./loading.gif");
        }
    };

    useEffect(() => {
        console.log("isHandwrite : ", isHandwrite);
    }, [isHandwrite]);

    return (
        <div class="upload-container" id="upload">
            <div class="upload-first-row">
                <p class="upload-title">Upload your picture</p>
                <div class="upload-file-container">
                    <img class="upload-file-icon" width="60" src={upload} alt="upload icon" />
                    <label for="file" class="upload-file-label">{filename !== "" ? filename : "Choose a file"}</label>
                    <input type="file" id="file" accept="image/*" class="upload-input" onChange={handleChange} />
                    <label for="handwriteCheck" style={{marginTop: 30}}>Is number handwritten?</label>
                    <input type="checkbox" id="handwriteCheck" name="handwritting" value={isHandwrite} onChange={() => setIsHandwrite(!isHandwrite)} />
                    <label for="hexaCheck" style={{marginTop: 30}}>Is hexa number?</label>
                    <input type="checkbox" id="hexaCheck" name="hexa" value={isHexa} onChange={() => setIsHexa(!isHexa)} />
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
                            <img class="upload-file-process-image" src={file} alt="preview" style={{alignSelf: "center"}}/>
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
                                {step === MAX_STEP && result !== [] && (result.map((item, index) => {
                                    return (
                                        <div class="result-list" key={index}>
                                            {index === 2 || index === 5 ?
                                                (item.map((item2, index2) => {
                                                    if (index2 === 2 || index2 === 5)
                                                    {
                                                        return (
                                                            <div class="result-list-item border-style-right border-style-bottom" key={index2}>
                                                                {item2["solved"] === 'true' ?
                                                                    <p class="result-list-item-title color-red">{item2["value"]}</p>
                                                                    : <p class="result-list-item-title">{item2["value"]}</p>
                                                                }
                                                            </div>
                                                        )
                                                    }
                                                    return (
                                                        <div class="result-list-item border-style-bottom" key={index2} >
                                                            {item2["solved"] === "true" ?
                                                                <p class="result-list-item-title color-red">{item2["value"]}</p>
                                                                : <p class="result-list-item-title">{item2["value"]}</p>
                                                            }
                                                        </div>
                                                    )
                                                }))
                                            :
                                                (item.map((item2, index2) => {
                                                    if (index2 === 2 || index2 === 5)
                                                    {
                                                        return (
                                                            <div class="result-list-item border-style-right" key={index2}>
                                                                {item2["solved"] === "true" ?
                                                                    <p class="result-list-item-title color-red">{item2["value"]}</p>
                                                                    : <p class="result-list-item-title">{item2["value"]}</p>
                                                                }
                                                            </div>
                                                        )
                                                    }
                                                    return (
                                                        <div class="result-list-item" key={index2} >
                                                            {item2["solved"] === "true" ?
                                                                <p class="result-list-item-title color-red">{item2["value"]}</p>
                                                                : <p class="result-list-item-title">{item2["value"]}</p>
                                                            }
                                                        </div>
                                                    )
                                                }))
                                            }
                                        </div>
                                    )
                                }))}
                            </div>
                            {step !== MAX_STEP &&
                                (<img
                                    class="upload-file-process-image"
                                    /* eslint-disable-next-line */
                                    src={doubleCheck()}
                                    alt="preview"
                                    style={{alignSelf: "center", minHeight: "100%"}}
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