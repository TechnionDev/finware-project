# Overview

This is a guide to help developers who want to modify and extend this project.

The purpose of the guide is to detail the structure of the project and explain where things are placed in the heirarchy and thus save time for the poor developer who's already probably on their 3rd cup of coffee today.

# Project Overview

The Finware project is intended to help raise financial awareness and help households maintain and track a budget.

Simply put, Finware puts the monthly expenses in clear sight.

The project is made of two parts, the E-Ink display (LilyGo T5) and the Raspberry Pi as (the brain of the operation). The E-Ink uses BLE to achieve low power consumption.

The E-Ink periodically (every 24 hours) pulls information from the near by Raspberry Pi (we'll short it to RPi from now) and updates the received information on the screen.

The RPi saves the user's credentials to their credit card companies (100% locally) and scans (scrapes) for transactions to analyze.

The RPi sums the expenses and creates graphs for the E-Ink to pull and display to the user.

The user is also able to hide/unhide the information on the E-Ink using a dedicated button. There's also the button to switch views (and ofc the "reset" button to refresh and force pull from the RPi).

# Feature List

The main (and some secondary that we're proud of) features are:

1. Display sum of expenses (with progress bar and warning when exceeding maximum)
2. Display graph of daily expenses (column for each day)
3. Display expenses per credit card
4. Add multiple credit card account (Max/Cal/Isracard/Amex/etc...)
   * Error screenshot when failing to scrape
5. Low energy consumption for the E-Ink - about 100mAh/month (achieved using BLE)
6. User accessible settings using a web interface
   1. WiFi configuration
   2. Bluetooth pairing
7. Security and Privacy (!!!)
   1. The web configuration is encrypted under an unique SSL certificate (generated during initial setup of the RPi).
      The certificate can be added to the user's devices to trust the portal as the valid page.
   2. Bluetooth secure pairing using pairing code on the E-Ink and manually entering it on the RPi's config page.
   3. Data is saved completely locally and not shared with any centralized server
   4. Data is 
8. Email notification on over budget expenses
9. Battery percentage indicator
10. Attention symbol on the E-Ink to indicate an error that requires user intervention
11. Multiple instances of the project in the same area

## Known Issues

Like everything in life, the project isn't perfect. There are a few missing/problematic things:

1. A base model RPi can be overloaded by manually force scraping many accounts
2. Pairing errors are hard to decipher
3. Only the first bit of the scanning process actually discovers new devices (if a device becomes discoverable about 5 seconds after scan starts, it wouldn't be detected by the E-Ink)

## Handled Edge Cases

1. Scheduled scraping will queue scraping tasks to mitigate load on the RPi
2. 

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
* `eink-display` - code base for the E-Ink LilyGo display
  * `main.cpp` - 
  * `PageManager.cpp` - 
  * <!--TODO…-->

## Examples For Adding Features

### Rescan button

The absolute majority of the time the E-Ink is in deep sleep to preserve battery.

You can read about deep sleep to understand the mechanism better.

To add another button you'd need another RTC GPIO pin.

`void handleClick(int gpiopin)` is the function that handles button clicks. It is being called for handling both wakeup clicks and clicks when the E-Ink is running fully awake.

So in that function a new `else if` for the new button needs to be added. The `else if` would call the handler of that rescan action.

Lastly, to make sure that the new GPIO can also wake up the E-Ink, you'd need to add it to the mask for the deep sleep wakeup (`BUTTON_PIN_BITMASK`).

### Multiple E-Ink for One RPi

Assuming a feature for multiple E-Ink displays for a single RPi such that each display would have a different set of accounts it watches.

#### Backend Changes

