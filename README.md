- [Overview](#overview)
- [Project Overview](#project-overview)
- [Feature List](#feature-list)
  * [Known Issues](#known-issues)
  * [Handled Edge Cases](#handled-edge-cases)
- [Project Structure](#project-structure)

<small><i><a href='http://ecotrust-canada.github.io/markdown-toc/'>Table of contents generated with markdown-toc</a></i></small>

# Project Overview

The Finware project is intended to help raise financial awareness and help households maintain and track a budget.

Simply put, the Finware project is an E-ink display that puts the monthly expenses in clear sight (like on the fridge door).

The project is made of two parts, the E-Ink display (LilyGo T5) and the Raspberry Pi as (the brain of the operation). The E-Ink uses BLE to achieve low power consumption.

The E-Ink periodically (every 24 hours) pulls information from the nearby Raspberry Pi (we'll short it to RPi from now) and updates the received information on the screen.

The RPi saves the user's credentials to their credit card companies (100% locally) and scans (scrapes) for transactions to analyze.

The RPi sums the expenses and creates graphs for the E-Ink to pull and display to the user.

The user is also able to hide/unhide the information on the E-Ink using a dedicated button. There's also the button to switch views (and ofc the "reset" button to refresh and force pull from the RPi).

This project was created by ([Saar Ofek](https://github.com/saar111), [Gur Telem](https://github.com/gur111), [Daniel Bondar](https://github.com/danibondar)) in the Technion University with guidance from the [Interdisciplinary Center for Smart Technologies](https://icst.cs.technion.ac.il/).

# Feature List

The main (and some secondary that we're proud of) features are:

1. Display sum of expenses (with progress bar and warning when exceeding maximum)
2. Display graph of daily expenses (column for each day)
3. Display expenses per credit card
4. Add multiple credit card account (Max/Cal/Isracard/Amex/etc...)
  * Error screenshot when failing to scrape
5. Low energy consumption for the E-Ink - about 100mAh/month (achieved using BLE)
6. User accessible settings using a web interface
7. WiFi configuration
8. Bluetooth pairing
9. Security and Privacy (!!!)
10. The web configuration is encrypted under a unique SSL certificate (generated during initial setup of the RPi).
    The certificate can be added to the user's devices to trust the portal as the valid page
11. Bluetooth secure pairing using pairing code on the E-Ink and manually entering it on the RPi's config page
12. Data is saved completely locally and not shared with any centralized server
13. Only relevant data is stored (within the dates range)
14. Email notification on over budget expenses
15. Battery percentage indicator
16. Attention symbol on the E-Ink to indicate an error that requires user intervention
17. Multiple instances of the project in the same area 
18. Scheduled scraping will queue scraping tasks to mitigate load on the RPi
## Known Issues

Like everything in life, the project isn't perfect. There are a few missing/problematic things:

1. A base model RPi can be overloaded by manually force scraping many accounts
2. Pairing errors are hard to decipher
3. Only the first bit of the scanning process actually discovers new devices (if a device becomes discoverable about 5 seconds after scan starts, it wouldn't be detected by the E-Ink)

# Project Structure

The folder hierarchy is as follows:

* `configuration-portal` - code base for the RPi.
  * `favicon` - icons for all the bank accounts
  * `public` - static files (icons, react base html, bank logos, etc...)
  * `scripts` - Scripts for developers to use manually (not used in production)
  * `security` - SSL related files
  * `src` - react (frontend) sources
    * <!--TODO…-->
  * `src-backend` - well.... it's the source for the backend.
    * `controllers` - (conceptual) singletons to handlers
      * `bluetooth-mock.controller.ts`
      * `bluetooth.controller.ts`
      * `financial-accounts.controller.ts`
      * `utils.ts`
    * `models` - database schemas (mongoose)
    * `routes` - routers to redirect requests to the correct handlers (login authorization checks)
    * `util-managers` - things that didn't fit within any of the controllers.
* `eink-display` - code base for the E-Ink LilyGo display. This component was developed using platformio.
  * `platformio.ini` - the platformio config file. all the esp32 libraries that were used in the project are mentioned there. 
  * `src` 
    * `main.cpp` - the main file of the E-ink. the setup and loop function are located there. 
    * `PageManager.cpp` - a class that manages the graphical aspects of the pages the E-ink displays. 
    * `BluetoothManager.cpp` - a class the manages the communication and information gathering with the RPI.
    * `utils.cpp` - a util file


