import React from 'react'
import "./Team.css";
import linkedin from "../assets/linkedin.png";
import armand from "../assets/team/armand.blin.jpeg";
import aurelien from "../assets/team/aurelien_daudin.jpeg";
import khaled from "../assets/team/khaled.mili.jpeg";
import maxim from "../assets/team/maxim.boc.jpeg";

const Team = () => {
    return (
        <div class="team-container">
            <p class="team-title">Meet the team</p>
            <div class="team-list-container">
            <div class="team-list-main">
                <div class="team-list">
                    <div class="team-list-item">
                        <img class="team-list-item-image" src={armand} alt="profile" />
                        <div class="team-right-content">
                            <p class="team-list-item-name">Armand BLIN</p>
                            <p class="team-list-item-role">Project Leader</p>
                            <a class="team-list-item-in-link" href="https://www.linkedin.com/in/armandblin/"><img src={linkedin} class="linkedin-ico" alt="linkedin"/></a>
                        </div>
                    </div>
                </div>
                <div class="team-list">
                    <div class="team-list-item">
                        <img class="team-list-item-image" src={aurelien} alt="profile" />
                        <div class="team-right-content">
                            <p class="team-list-item-name">Aurélien DAUDIN</p>
                            <p class="team-list-item-role">Developer</p>
                            <a class="team-list-item-in-link" href="https://www.linkedin.com/in/aurelien-daudin/"><img src={linkedin} class="linkedin-ico" alt="linkedin"/></a>
                        </div>
                    </div>
                </div>
            </div>
            <div class="team-list-main">
                <div class="team-list">
                    <div class="team-list-item">
                        <img class="team-list-item-image" src={maxim} alt="profile" />
                        <div class="team-right-content">
                            <p class="team-list-item-name">Maxim BOCQUILLION</p>
                            <p class="team-list-item-role">Developer</p>
                            <a class="team-list-item-in-link" href="https://www.linkedin.com/in/maxim-bocquillion-07a56b1a6/"><img src={linkedin} class="linkedin-ico" alt="linkedin"/></a>
                        </div>
                    </div>
                </div>
                <div class="team-list">
                    <div class="team-list-item">
                        <img class="team-list-item-image" src={khaled} alt="profile" />
                        <div class="team-right-content">
                            <p class="team-list-item-name">Khaled MILI</p>
                            <p class="team-list-item-role">Developer</p>
                            <a class="team-list-item-in-link" href="https://www.linkedin.com/in/khaled-mili-8940a922b/"><img src={linkedin} class="linkedin-ico" alt="linkedin"/></a>
                        </div>
                    </div>
                </div>
            </div>
            </div>
        </div>
    )
}

export default Team