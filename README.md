# item_maker

## For end-user:

### Description
Basically, it's updated version of [phantomwolf's AI:TG Item Maker exp guide](http://tinyurl.com/aitgitemmaker).
Since it's very outdated, I've decided to not only update it, but make the whole (well, almost whole) process automated:
- you take source code (either from [Omesh](https://drive.google.com/drive/u/0/folders/0B2cvINBa4g7WakZUck9lT24zUVk) or by decompiling game's swf yourself);
- you run first program, item_maker_api.exe: you select the source code, after a while select output csv file and you get csv file containing all items' values (aside from medals) along with "IM exp per resource" statistics;
- then (if you satisfy conditions explained in \***Technical info**\* section), by using second program, item_maker_excel.exe, you can convert said csv file into Excel spreadsheet! The process is similiar to first program, and resulting Excel file has frozen first row and column for ease of use, as well as sortable values.

**If you don't care and just want to see the values in question, I will try to put compiled .exe files and resulting csv and xlsx files in _Release_ tab in case of updates of Anti-Idle or the programs themselves. That is, as long as I'm interested in AI:TG and/or this project. (That is one of the reasons why this thing is on Github)**

**Also, at the end of this README.md should be some sort of list of links to cloud drive, where spreadsheets will be stored. (The ULTRA lazy version).**

### Notes
- You might notice that (unlike pw's guide) this one does not generate summaries or ranks. Instead, I decided that you should be able to search through data yourself and pick best option depending on your resources.
- Because Excel spreadsheet is automatically generated:
  - Formatting is (rather) poor. That can be circumvented by creating your own Excel spreadsheet from provided CSV file;
  - There might be some factual errors (there should not be any, but who knows?). Feedback is much appreciated!
- Numerical values are represented in scientific format, because you can then multiply "IM exp per ???" values by powers of 10 easily (hopefully). If you dislike the format you can freely change it.
- If you are wondering why there are two programs instead of just one, item_maker_api.exe is written in C++ and item_maker_excel.exe is written in C#/.NET; fusing these two would be real pain in the back.
- If the program doesn't work, please issue an issue in **Issues** tracker. The error might be caused by:
  - Tukkun breaking structure of the code;
  - your PC not satisfying **Requirements**;
  - something machine-dependent I couldn't possibly predict.
- If you want to suggest something, you can use **Issues** or forum thread (I know **Issues** should not be used for that, but whatever). The forum thread is also perfect place for user-made charts and rankings.
- If there are grammatical errors within this README.md, point them out (**Issues** again?), but don't be grammar Nazi.
- Your kind words of appreciation are appreciated :)

## Technical info:

### Target functionalities of programs:
**item_maker_api.exe:**
- asks for source code file;
- converts it into csv (comma-separated value);
- returns csv into player-defined file.
  
**item_maker_excel.exe:**
- asks for csv file created by item_maker_api.exe;
- converts it into excel file (.xlsx).

### Requirements:
- Windows (I've Windows 10 and have not tested on previous releases, but I'm positive it works on these as well);

**item_maker_api.exe:**
- no other requirements (I think).

**item_maker_excel.exe:**
- Microsoft Excel;
- .NET Framework (compiled for .NET 4.0).

***Warning: I think these requirements are correct; I will determine exact requirements when I have access to another machine with Excel.***

## Links list:
- [folder with generated spreadsheets](https://drive.google.com/drive/folders/0B3BzoLHkt-KAeE5kRzNEMk45amM)
