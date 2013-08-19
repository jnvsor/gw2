##Obsolete##
Made obsolete in 5 hours flat by /u/nabarok at reddit.

He [observed](http://www.reddit.com/r/Guildwars2/comments/1ko24h/i_wrote_a_program_that_gives_you_a_last_online/cbr0lzp) the requests the page's ajax made and noted they use a simple get parameter for the data. The end result is that you just stick `?pjax=1` on the end of the url, copy paste the result to a text file, and apply these regexes I made below.

    [0-9]+%\s+(.+\.[0-9]{4})\s+([0-9]+)( [0-9]+)? Since ([0-9]+)/([0-9]+)/([0-9]+).+

Replace with:

    \1,\2,\5,\4,\6

Or in a terminal:

    sed -ri 's/[0-9]+%\s+(.+\.[0-9]{4})\s+([0-9]+)( [0-9]+)? Since ([0-9]+)\/([0-9]+)\/([0-9]+).+/\1,\2,\5,\4,\6/g' $filename

Needless to say I'm a bit miffed at the simplicity of this method, given the work I put into my xml-parsing pattern-matching but I'm leaving the program here as it is for portfolio and/or in case anet "plugs the hole"

gw2rip
======
Pulls data from the GW2 achievement leaderboards and outputs it in csv format to show "Last logged in" times.

###Getting the html###
Go to http://leaderboards.guildwars2.com/en/na/achievements and log in. Filter by guild and select the guild you want to show. The page will now load all the members from your guild to the page.

####In firefox####
* Rightclick the table
* Select inspect element
* Browse to the `<tbody>` element
* Rightcilck the `<tbody>` element
* Select copy outer html
* Paste into file

####In chrome####
* Rightclick the table
* Select inspect element
* Browse to the `<tbody>` element
* Rightcilck the `<tbody>` element
* Select copy as html
* Paste into file

####In IE####
* Slap yourself
* Download [firefox](http://www.mozilla.org/firefox) or [chrome](http://www.google.com/chrome)
* Follow above instructions


###Compiling###
Compile the program - dependancies are [libxml2](http://www.xmlsoft.org/)

###Running###
Run it with the file containing the html as the first argument, and pipe the output to a csv file. eg:

    ./gw2rip ~/Documents/rippedhtml > ~/Documents/guild.csv

Open with your favorite spreadsheet software and sort or filter at will.
