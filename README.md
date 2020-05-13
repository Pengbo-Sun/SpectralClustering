# Spectral Clustering Algorithmen

## Examples 
Beinhaltet verschiedene Beispielordner, wobei jeweils eine datei namens "all.txt" als Eingang und eine Datei "list.txt" als Ausgang dient.

## Project
Beinhaltet die Spectral Clustering Algorithmen für entweder CPP oder Matlab

## Funktionsweise

Das Projekt "SpecMatlab" dient als Beispielimplementierung für das Spectral Clustering Verfahren in Matlab bzw. der Unterstützung von CPP, um eine Kommunikation zwischen der bereits bestehenden Implementierung zu ermöglichen

Das Projekt "SpecCPP" beinhaltet das Rohprogramm, um die Matlabprogrammierung in CPP zu implementieren. Hierzu soll die Headeronly-BoostLibrary verwendet werden, die sich bereits im Third-Party Ordner befindet. MitHilfe der initialen CMake Datei und den weiteren Programmteilen ist es derzeit möglich eine Datei einzulesen und den Graphen auszugeben.

Die Beispieldateien "all.txt" dienen als Input. Sie ist eine CSV-Datei mit folgendem Aufbau: "Anfangsnode","Endnode","Gewicht bzw. Ähnlichkeit". Der Output des Programms ist eine Datei ähnlich zu "list.txt". Diese beinhaltet eine Spalte, wobei jeder existierenden oder nichtexistierenden Node eine Gruppen-ID zugeteilt wurde. Die Gruppen-ID kann frei gewählt werden, jedoch sollten alle unbekannten bzw. übrigen Nodes mit einer 1 gekennzeichnet werden.

In "SpecCPP" sind folgende Funktionen zu implementieren:
Konstruktor
Destruktor
readin - Einlesen einer CSV-Datei
convert2graph - Konvertierungen der CSV-Datei in einen Graphen
getsubgraphs - Ermittlung aller "Connected" Grpahs innerhalb des Hauptgraphen
getmatrix - Ermittlung der sogenannten Laplace-Matrix für jeden Graphen(+ Gewichts- und Degree- Matrix, da sie ein Zwischenprodukt darstellen)
calceigen - Berechnung der Eigenwerte der Laplace-Matrix
calcgroups - Berechnung/Ermittlung der verschiedenen Untergruppen in jedem Graph nach dem Vorbild von http://www.scitepress.org/DigitalLibrary/Link.aspx?doi=10.5220/0006549303150322
savegraph - Abspeicherung der graphen in unterschiedlichen CSV-Dateien
savegroups - Abspeicherung der ermittelten Untergruppen

Weitere Funktionen können nach belieben implementiert werden.
