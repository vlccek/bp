# Typografie a publikování - 1. projekt
# Jakub Vlk
# xvlkja07stud.fit.vutbr.cz

NAME = proj2
LOGIN = xvlkja07


all:
	latex $(NAME).tex
	latex $(NAME).tex
	dvips -t a4 $(NAME).dvi
	ps2pdf -sPAPERSIZE=a4 $(NAME).ps

zip:
	zip $(LOGIN)-fit.zip *.tex Makefile

clean:
	rm -f *.{aux,dvi,log,ps,fls,fdb_latexmk,te~,gz,out} $(LOGIN)-fit.zip

# creates overlay vzorr.pdf $(NAME)
over: all
	pdftk vzor.pdf multibackground $(NAME).pdf output overlay.pdf