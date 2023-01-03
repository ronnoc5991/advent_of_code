import sys

if not len(sys.argv) == 3:
    print("Usage: ./count_calories filename number_of_elves_to_count")
    sys.exit()

filename = sys.argv[1]
elves_to_count = sys.argv[2]

highest_calorie_counts = [0] * int(elves_to_count)
current_elf_calorie_count = 0

with open(filename) as file:
    for line in file:
        # end of current elf
        if line in ['\n']:
            for i in range(int(elves_to_count)):
                if current_elf_calorie_count > highest_calorie_counts[i]:
                    for j in range(int(elves_to_count) - 1, i, -1):
                        highest_calorie_counts[j] = highest_calorie_counts[j - 1]
                    highest_calorie_counts[i] = current_elf_calorie_count
                    break
            current_elf_calorie_count = 0
        else:
            current_elf_calorie_count += int(line)

print(sum(highest_calorie_counts))