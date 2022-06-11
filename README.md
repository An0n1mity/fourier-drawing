<p align="center">
  <a href="" rel="noopener">
 <img width=200px height=200px src="https://www.tomesoftware.com/wp-content/uploads/2019/09/Screen-Shot-2019-07-09-at-10.50.35-AM-1-768x616.png" alt="Project logo"></a>
</p>

<h3 align="center">Fourier Drawing</h3>

<div align="center">

</div>

---

<p align="center"> Simple program showing a visual approach to the Fourier Series.
    <br> 
</p>

## 📝 Table of Contents

- [About](#about)
- [Installation](#installation)
- [Screenshot](#screenshot)
- [Acknowledgments](#acknowledgement)

## 🧐 About <a name = "about"></a>

This project was made as a demonstrative and visual uses of the this powerful mathematical concept that is the Fourier Series. 

The program gets the data from a SVG passed by the user, and then process it, resulting in a beautiful approximative drawing of the original SVG.

Disclaimer : The parsing module for SVG is not robust, program might crash on most of the files. 

## 🚀 Installation <a name = "installation"></a>
This project is built using those following libraries :

```
- libxml2 
- portaudio
- gtk3 
```

### Linux
Installing libraries

```
sudo apt-get install libxml2-dev libasound-dev libgtk-3-dev
```

Cloning and building 
```
git clone https://github.com/An0n1mity/fourier-drawing
mkdir build 
cd build
.. cmake
```

## 📷 Screenshot <a name = "screenshot"></a>
![alt text](https://github.com/An0n1mity/fourier-drawing/blob/main/screenshot.png)

## 🎉 Acknowledgements <a name = "acknowledgement"></a>

A great thanks to those great youtube channels that explained the mathematicals conecpts behind Fourier drawing.
<a>https://www.youtube.com/watch?v=uazPP0ny3XQ</a>
<a>https://www.youtube.com/watch?v=r6sGWTCMz2k</a>
