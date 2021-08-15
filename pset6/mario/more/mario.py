# Makes sure it only gets valid values
while True:
    # Gets height of blocks
    height = input("Introduce height: ")
    if height.isdigit() == False:
        continue
    if int(height) > 0 and int(height) < 9:
        break
# Prints tower
for i in range(1, (int(height) + 1)):
    print(" " * (int(height) - i) + "#" * i + "  " + "#" * i)
    