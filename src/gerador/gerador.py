from ast import dump
from faker import Faker
import pickle

faker = Faker()

try:
    file = open("file.txt", "w")

    for i in range(12):
        key = faker.unique.random_int()
        number = faker.random_int()
        name = faker.name()

        file.write((str(key) + " " + str(number) + " " + str(name) + "\n"))

    file.close()
    
except Exception as e:
    print(e)
