gw2rip
======

Pulls data from the GW2 achievement leaderboards and outputs it in csv format to show "Last logged in" times.

Getting the html
----------------

In firefox:
* Rightclick the table
* Select inspect element
* Browse to the <tbody> element
* Rightcilck the tbody element
* Select copy outer html
* Paste into file

In chrome:
* Rightclick the table
* Select inspect element
* Browse to the <tbody> element
* Rightcilck the tbody element
* Select copy as html
* Paste into file

In IE:
* Slap yourself
* Download firefox or chrome
* Follow above instructions


Compiling
---------

Compile the program - dependancies are libxml2

Running
-------

Run it with the file containing the html as the first argument, and pipe the output to a csv file. eg:

./gw2rip ~/Documents/rippedhtml > ~/Documents/guild.csv

Open with your favorite spreadsheet software and sort or filter at will.

