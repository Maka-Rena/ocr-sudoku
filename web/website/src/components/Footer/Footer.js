import React from 'react'
import epita from "../assets/epita.png";
import "./Footer.css";

const Footer = () => {
  return (
    <div class="footer-container">
        <p class="footer-text">Made with love by the Maka Rena Team!</p>
        <img src={epita} class="epita-logo" width="70" alt="epita logo"/>
        <p class="footer-text">Copyright © 2022</p>
    </div>
  )
}

export default Footer