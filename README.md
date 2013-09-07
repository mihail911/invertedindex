Inverted Index
================================

***Background***
This project was born out of my desire to play around with some basic information retrieval algorithms after reading Chris Manning's "Introduction to Information Retrieval." This program implements a version of the inverted index algorithm that lies at the heart of many search engines' functionality. For information on the algorithm, you can refer to http://en.wikipedia.org/wiki/Inverted_index. Given a collection of text files, this program finds the files containing a certain query string using inverted indices.

***Use***
Place the ```Makefile``` and ```invertedindex.cpp``` files in the same directory. Also include the text files you wish to use for your query in the same directory as the first two files.
Run ```make``` from terminal. Afterwards, run the ```invertedindex``` executable through the command ```./invertedindex```. Follow the instructions given by the program.