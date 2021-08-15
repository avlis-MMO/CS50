def main():

    text = input("Input your text: ")
    
    # Counts the required information
    letters = count_letters(text)
    words = count_words(text)
    sentences = count_sentences(text)

    # Calculate the values of L and S used in the function given
    L = letters * 100.0 / words
    S = sentences * 100.0 / words
    Grade = round(0.0588 * L - 0.296 * S - 15.8)
    
    # Prints the grade in the format asked
    if Grade >= 16:
        print("Grade 16+")
    
    elif Grade < 1:
        print("Before Grade 1")
    
    else:
        print("Grade " + str(Grade))
        

def count_letters(text):
    
    number_letters = 0
    n = len(text)
    
    # Counts all the letters in the text
    for letter in text:
        
        # Checks if it is a letter or not
        if letter.isalpha():
            number_letters += 1

    return number_letters
    

def count_words(text):
    
    number_words = 0
    n = len(text)
    
    # Counts all the words in the text
    for i in range(n):

        # Checks if it is a space or end of text
        if text[i] == ' ' or text[i] == '\0':
            number_words += 1 
    
    return number_words + 1
    

def count_sentences(text):

    number_sentences = 0
    n = len(text)
    
    # Counts all the sentnces in the text
    for i in range(n):

        # Checks if it is '.', '?', '!'
        if text[i] == '.' or text[i] == '?' or text[i] == '!':
            number_sentences += 1
    
    return number_sentences
    
    
main()