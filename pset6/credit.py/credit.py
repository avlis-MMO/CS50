arr2_number = []
arr3_number = []
txt = ""
two_digits = ""
Sum = 0

while True:
    # Get number 
    number = input("Introduce card number: ")
    # makes sure it is a digit
    if number.isdigit():
        break
# Creates a list with ll the digits  
arr_number = list(number)

# Gets the first two digits
two_digits = str(arr_number[0]) + str(arr_number[1])
two_digits = int(two_digits)

# Gets number of digits
n = len(arr_number)

# Multiplies all the digits by two
for i in range(1, int(n / 2) + 1):
    arr2_number.append(int(arr_number[n - (2 * i)]) * 2)
# Sums the digits not multiplided by two   
for i in range(0, round(n/2)):
    Sum = Sum + (int(arr_number[n - (2 * i + 1)]))

# Transforms the multiplied values in single digits, 12 -> '1', '2' 
for i in arr2_number:
    txt = txt + str(i)
arr2_number = list(txt)

# Sums the remaining digits
for i in range(0, len(arr2_number)):
    Sum = Sum + int(arr2_number[i])

if n == 15 and (two_digits == 34 or two_digits == 37) and Sum % 10 == 0:
    print("AMEX\n")
    
elif n == 16 and (two_digits >= 51 and two_digits <= 55) and Sum % 10 == 0:
    print("MASTERCARD\n")
    
elif (n >= 13 and n <= 16) and (two_digits >= 40 and two_digits <= 49) and Sum % 10 == 0:
    print("VISA\n")
        
else:
    print("INVALID\n")
