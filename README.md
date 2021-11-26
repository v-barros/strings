# strings
A simplified implementation of Strings and StringTable, based on OpenJDK.

The idea behind this project is to provide a deeper understanding of JVM internals and how Strings are - elegantly - managed in run time to save memory.

## String creation, wich can be done,in Java, by 2 different ways.
In general, we can define these ways as:
    String object = Strings calculated in run time.
    String literal = Strings defined in source code.
   
  String Object:
  When using the constructor ( **new String()** ), we're creating a new String Object. 
  Usually, the methods returning a String are actually creating a new String
  Object, like substring(), concat(),valueOf(),toString() etc; 
  
  String Literal: When using double commas ( **String str = ""** ) we're creating a String Literal;
  
  Only String Literals are automatically interned, i.e., the JVM 
    performs a Lookup on the **_String table_** and if the string we're 
    trying to create has already been interned, then the local variable 
    is going to point to a reference to this pre-existing string.
    Example:
```
String a = "foo"; // run a lookup and finds nothing, intern "foo"
String b = "foo"; // run a lookup and find a existing string "foo", return pre-existing "foo" to variable b
```
  Both variables are pointing to the same object, so there is just one
    instance of "foo", living in **_String Table_**.
   
  String Objects don't get interned automatically, this is, when we 
    use the constructor, the JVM creates a new string even if
    there is already an equal string on string table.
    Example
```
 String a = "foo";
 String b = new String("foo");
```
   In this case, there are two different instances of String, where
   a has a reference to an interned string on string table and 
   b holds a reference to an string located anywhere inside the heap.

## String Table
String Table is a Hash Table used to store Java Strings.

As mentioned above, they can be usefull in scenarios with duplicated Strings.
This approach gives us a possibility to save a huge amount of memory.
By default, the string table is set to 65536 buckets( run **java -XX:PrintFlagsFinal** and search for **StringTableSize** to check ), but by increasing this value, the table bucket gets less populated and overrall performance is increased.


