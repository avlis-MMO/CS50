import sys
import csv
import re


def main():
    # makes sure thre is 3 argumments
    if len(sys.argv) != 3:
        sys.exit("Usage: python dna.py data.csv text.txt")
    people = []
    
    # reads the file
    with open(sys.argv[1]) as file:
        reader = csv.DictReader(file)
        # gets the header of the file
        header = reader.fieldnames
        # crates a list with all the people
        for name in reader:
            people.append(name)
    
    counter = {}
    
    # creates a dict with the occorrunce of STR
    for i in range(1, len(header)):
        counter[header[i]] = 0
        
    # Counts the STR's in a sequence
    for STR in counter:
        value = count(STR)
        counter[STR] = value
    
    # finds the person
    name = find_person(people, counter)
    print(name)
    
    
def count(STR):
    
    with open(sys.argv[2]) as file:
        lines = str(file.readlines())
    
    # finds all the consecutive sequences od STR
    pattern = "(?=((" + STR + ")+))" 
    groups = re.findall(pattern, lines)
    
    # returns the highest number of consecutive STR
    if groups:
        return (max(len(m[0]) // len(STR) for m in groups))
    else:
        return 0
        
    
def find_person(people, counter):
    
    flag = True
    
    # checks if it is the right person or not
    for i in range(len(people)):
        for STR in counter:
            if int(people[i][STR]) == counter[STR]:
                flag = True
            else:
                flag = False
                break
        if flag:
            return people[i]['name']
        else:
            continue
    return 'No match'
    
         
main()
