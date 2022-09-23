<<<<<<< HEAD
- [Installation](#installation)
  * [Setup Raspberry Pi](#setup-raspberry-pi)
    + [Step 1 - Installing our image on your microSD card (microSD must be `TODO` GB or more)](#step-1---installing-our-image-on-your-microsd-card--microsd-must-be--todo--gb-or-more-)
    + [Step 2 - Power on your Raspberry Pi](#step-2---power-on-your-raspberry-pi)
    + [Step 3 - Connect Raspberry Pi to WiFi](#step-3---connect-raspberry-pi-to-wifi)
    + [Done!](#done-)
  * [Setup Credit Cards & Configuration](#setup-credit-cards---configuration)
  * [Setup the E-Ink display](#setup-the-e-ink-display)

<small><i><a href='http://ecotrust-canada.github.io/markdown-toc/'>Table of contents generated with markdown-toc</a></i></small>

# Installation
## Setup Raspberry Pi
Firstly we will setup the Raspberry Pi and the server with which the E-Ink display will communication with.
### Step 1 - Installing our image on your microSD card (microSD must be `TODO` GB or more)
1. Download [our image](https://TODO-ADD-LINK) to your computer
2. **Launch Raspberry Pi Imager** on your PC. You can [download Raspberry Pi Imager](https://www.raspberrypi.org/downloads/) if you don’t have it already

<img src="https://user-images.githubusercontent.com/10295509/191362481-41db7a9a-08e1-4317-97af-06ec33cb034a.png" width="400" />

3. **Select Use custom** from the Choose OS menu

<img src="https://user-images.githubusercontent.com/10295509/191362902-4f0d1c94-cff2-4b77-ad20-2ff5bf817373.png" width="400" />

4. **Select our image** from your downloads folder
5. **Select the microSD card** you wish to burn the image to

<img src="https://user-images.githubusercontent.com/10295509/191363100-c83d277d-e912-48a9-abf5-6b8ee565d9af.png" width="400" />

6. **Click Write**

<img src="https://user-images.githubusercontent.com/10295509/191363488-13f854ac-b56f-4152-afa4-a50939c75f45.png" width="400" />

### Step 2 - Power on your Raspberry Pi
1. Insert the burnt microSD into your Raspberry Pi
2. Connect the Raspberry Pi to a power source


### Step 3 - Connect Raspberry Pi to WiFi
1. Connect to the "Finware" WiFi access point from your smart phone, using the password: **`finware!`**
2. Open your browser on your smartphone and navigate to https://raspberrypi.local/wifi\
<img src="https://user-images.githubusercontent.com/10295509/191369338-4b30bade-0eed-48e6-ac03-567aa8baa341.png" width="200" />

3. Select your WiFi network from the list, enter your password and submit


### Done!
That's it, everything in the Raspberry Pi is configured. You can make sure it is up and running by navigating to https://raspberrypi.local/

## Setup Credit Cards & Configuration
The next step will be to add the credentials of each credit card you want to monitor
1. Connect to the same WiFi as the one you have configured for the Raspberry Pi
2. Navigate to https://raspberrypi.local/ and select the "Credit Cards" menu item
<img src="https://user-images.githubusercontent.com/10295509/191369689-531e10db-7d55-4666-802d-ef80146c9f0d.png" width="200" />

3. Press the `Add Account` button
<img src="https://user-images.githubusercontent.com/10295509/191369824-025d2f4c-50c6-4e4b-b35d-914dccf413f5.png" width="200" />

4. Pick your credit card company, enter the relevant credentials and press `Create`
<img src="https://user-images.githubusercontent.com/10295509/191370071-1e91a66a-c002-41e5-b601-7d1e2ca7d6e2.png" width="200" />

5. Navigate to https://raspberrypi.local/ and select the "General Config" menu item
6. Change any setting you want to be different from the default

## Setup the E-Ink display
//TODO...




<!-- ## E-Ink Display -->
=======
Welcome to the **Finware** project.\
Please refer to the [Wiki](https://github.com/TechnionDev/finware-project/wiki) for setup and installation information


Please note that this page and wiki are still under construction - thank you for your understanding!
>>>>>>> caaa35bd85d8fad4ac892d617aaad72634982ea1
