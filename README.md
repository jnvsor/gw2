gw2rip
======

Pulls data from the GW2 achievement leaderboards and outputs it in csv format to show "Last logged in" times

gw2rip takes the html content of the leaderboards table for a guild, and returns
a csv list of account names, achievement points, and dates last achieved.

The html content is crawled as xml, and requires a root element of type tbody.

Getting the html:
In firefox
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

Compile the program - dependancies are libxml2

Run it with the file containing the html as the first argument, and pipe the
output to a csv file. eg:

./gw2rip ~/Documents/rippedhtml > ~/Documents/guild.csv

Open with your favorite spreadsheet software and sort or filter at will.
