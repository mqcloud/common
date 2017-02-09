SET PATH=%cd%;%cd%\..\Tools\Graphviz\bin\;%cd%\..\Tools\Python\;%cd%\..\Tools\Python\Scripts\;%cd%\..\Tools\Latex\;%path%

SET NAME=test

taskkill /im acrobat.exe
DEL /F /Q %NAME%.aux
DEL /F /Q %NAME%.bbl
DEL /F /Q %NAME%.blg
DEL /F /Q %NAME%.dvi
DEL /F /Q %NAME%.log
DEL /F /Q %NAME%.pdf
DEL /F /Q %NAME%.ps

pdflatex --enable-write18 -shell-escape %NAME%.tex
bibtex %NAME%.aux
pdflatex --enable-write18 -shell-escape %NAME%.tex
pdflatex --enable-write18 -shell-escape %NAME%.tex


DEL /F /Q %NAME%.aux
DEL /F /Q %NAME%.bbl
DEL /F /Q %NAME%.blg
DEL /F /Q %NAME%.dvi
DEL /F /Q %NAME%.ps