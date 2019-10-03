# CodePreview
Preview handlers for Microsoft Windows and Office to preview source code files.
**This is old code I wrote in November 2011. I am not actively maintaining this project anymore. I moved it to GitHub to make sharing easier.**

## Original description
Windows Vista/7 and Microsoft Office Outlook 2007/2010 have previews for certain files without having the need to open them. Some previews are build-in like so you can instantly view Word, PowerPoint or Excel files. Some other common file types that are regularly used as email attachments, like audio files or videos, are also supported.

As a software developer, I regularly get mails with some sort of source code attached to them. Since there are no build-in previewers for these files, I need to open these files in my IDE to view them. That's an annoying thing since I often only want to take a quick look what I got. So I wrote some preview handlers for these file types.

Since I didn't want to reinvent the wheel I used [Scintilla](https://www.scintilla.org/) as source code viewer. It saved me a whole lot of work and without Scintilla I wouldn't have written CodePreview.

There are some other projects out there that try to tackle the same problem (like [this](http://previewhandlers.codeplex.com/) or [this](http://www.mydigitallife.info/download-cs-vb-js-and-sql-files-code-preview-handler-for-vista-and-outlook-2007/) project). They all are built upon an [excellent article by Stephen Toub](https://web.archive.org/web/20111224170846/http://msdn.microsoft.com/en-us/magazine/cc163487.aspx). These handlers use the .NET-Framework to implement their preview functionality.

CodePreview instead is written in native code so there is no need for the .NET-Framework.

## How to install
To install a preview handler, simply copy its DLL to some folder from where code can be executed like `C:\Program Files (x86)\CodePreview`.

Open a terminal with administrative rights and navigate to the folder with the DLL. Install the DLL by executing `regsvr32 /i [DLL]`. You should get a message telling you that the DLL was successfully installed. You can instantly use the preview handler, no restart needed.

To uninstall do the same as above but with the command `regsvr32 /u [DLL]`.
