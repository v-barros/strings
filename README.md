# strings
A simplified implementation of Strings and StringTable, based on OpenJDK.

For this project,there is no concern to make it proper - yet -
for an "end-user" usage,so there is not a really well defined
edge between stringtable - string - end-user (main.c, in this case).
A better aproach for this would be just like Java, which is:
Java_Lang_String -> Symbol -> SymbolTable -> HashTable, in the hash
table we don't even know what is a Java_Lang_String, and for the
programmer using the String class, they don't know and they can't
access the String Table, besides when explicitly calling intern(),
but this would be way harder to achieve using C, as these are OOP
concepts to abstractions.
The idea is to improve it further and make it as usable as 
possible, but, for now, its like a beta version and helped me
to understand a little bit of how Strings actually work in Java. 
I hope this project can be helpful for someone else.



