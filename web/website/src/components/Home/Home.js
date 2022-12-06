import React from 'react'
import "./Home.css"
import term from "../assets/solver.png";
import sudoku from "../assets/sudoku.png";

const Home = () => {
  return (
    <div class="home-container">
        <div class="home-left-content">
            <img src={sudoku} alt="sudoku"  class="sudoku-ico"/>
            <p class="home-title">Sudoku Solver</p>
            <p class="home-subtitle">Solve your sudoku with a picture!</p>
            <a href="#upload" class="home-button">Get Started</a>
        </div>
        <div class="home-right-content">
            <img src={term} alt="Sudoku Solver" class="home-image"/>
        </div>
    </div>
  )
}

export default Home