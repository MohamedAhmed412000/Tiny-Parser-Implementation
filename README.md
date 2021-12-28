<div id="top"></div>
<h1 align="center"> Tiny Compiler Implementation </h1>
<br />
<div align="center">
  <a href="https://github.com/MohamedAhmed412000/Tiny-Parser-Implementation">
    <img src="https://github.com/MohamedAhmed412000/Tiny-Parser-Implementation/blob/main/Images/ide.png" alt="Logo" width="200">
  </a>

  <p align="center">
    **This repo contains implementation for scanner and parser of Tiny language**
    <br />
    <a href="https://drive.google.com/file/d/1fbZ4UPeEFWRtvVoTSbfc7to7DXOMfr-G/view?usp=sharing"><strong>Explore the docs »</strong></a>
</div>



<!-- TABLE OF CONTENTS -->
<details>
  <summary>Table of Contents</summary>
  <ol>
    <li>
      <a href="#about-the-project">About The Project</a>
      <ul>
        <li><a href="#built-with">Built With</a></li>
      </ul>
    </li>
    <li>
      <a href="#getting-started">Getting Started</a>
      <ul>
        <li><a href="#prerequisites">Prerequisites</a></li>
        <li><a href="#running-executable">Running Executable</a></li>
      </ul>
    </li>
    <li><a href="#usage">Usage</a></li>
    <li><a href="#features">Features</a></li>
    <li><a href="#license">License</a></li>
  </ol>
</details>


<!-- ABOUT THE PROJECT -->
## About The Project

<img src="https://github.com/MohamedAhmed412000/Tiny-Parser-Implementation/blob/main/Images/1.PNG" alt="Scanner">
<img src="https://github.com/MohamedAhmed412000/Tiny-Parser-Implementation/blob/main/Images/2.PNG" alt="Parser">
<img src="https://github.com/MohamedAhmed412000/Tiny-Parser-Implementation/blob/main/Images/3.PNG" alt="Parse Tree Draw">


<p align="right">(<a href="#top">back to top</a>)</p>



### Built With

* [CPP](https://www.cplusplus.com/)
* [QT5 framework](https://www.qt.io/)
* [Graphviz library](https://www.graphviz.org/)


<p align="right">(<a href="#top">back to top</a>)</p>



<!-- GETTING STARTED -->
## Getting Started

### Prerequisites

If you will run the code you need to install QTCreator and Graphviz library and add it to your debug project path

* [QT download](https://www.qt.io/download)
* [Graphviz download](https://graphviz.org/download/)


### Running Executable

Nothing needed only Clone the GitHub repo and in the Executable folder the .exe file will be found

<p align="right">(<a href="#top">back to top</a>)</p>



<!-- USAGE EXAMPLES -->
## Usage

Tiny language is inherited from C language so it's a good idea to begin understanding how compiler works with its syntax

<p align="right">(<a href="#top">back to top</a>)</p>



<!-- Features -->
## Features

1. Scanner
	- Import Tiny Code file
	- Generate Tokens and save result in tokens.txt file
	- Handeling some errors like "Identifier must start with characters, Incomplete Comment"
2. Parser
	- Import Tokens file in this format ``` x, IDENTIFIER ``` or paste it in Tokens Text Edit
	- Check Validity of tokens (Wrong Token Number is shown)
	- Draw Parse Tree
	- Save Parse Tree Image in SVG Format
	- Auto Save Images

<p align="right">(<a href="#top">back to top</a>)</p>



<!-- LICENSE -->
## License

Distributed under the MIT License. See `License.txt` for more information.


© 2021 GitHub, Inc.
