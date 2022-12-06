import React from 'react'
import "./Header.css"
import logo from "../assets/logo_short.png";

const Header = () => {
  return (
    <div class="header-container">
        <div class="header-name-container">
            <img src={logo} alt="Netflix Logo"  class="logo"/>
            <p class="group-name">MAKA RENA</p>
        </div>
        <div class="header-github-container">
            <a href="https://github.com/Maka-Rena/ocr-sudoku" target="_blank" class="github-link" rel="noreferrer">
                GitHub
            </a>
        </div>
    </div>
  )
}

export default Header;