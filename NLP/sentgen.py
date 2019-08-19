import nltk
import random

# bigrams + coherence

def load_bigrams(text_str):
    words = text_str.split()
    bigrams = list(nltk.ngrams(words, 2))
    return bigrams

def new_word(word, list, POS):
    for (a, b) in list:
        b_list = [b]
        if word == a:
            tag = nltk.pos_tag(b_list)
            if POS == tag[0][1]:
                return b
    for (a, b) in list:
        b_list = [b]
        if word == a:
            return(b)

def is_punct(letter):
    return letter == "." or letter == "!" or letter == "?" or letter == "..." or letter == '"'

def find_sentence(token):
    start = random.randrange(0,len(token))
    count = 0
    period1 = 0
    for i in range (start, len(token)):
        if len(token[i]) >= 1:
            if is_punct(token[i][-1]):
                count += 1
                if count == 1:
                    period1 = i
            if count == 2:
                return token[period1+1: i+1]


filename = "/Volumes/COURSES/cs099-00-su19/CourseMaterials/gutenberg/data/text/PG34950_text.txt"
file = open(filename)
filename2 = "/Volumes/COURSES/cs099-00-su19/CourseMaterials/gutenberg/data/text/PG17_text.txt"
file2 = open(filename)
filename3 = "/Volumes/COURSES/cs099-00-su19/CourseMaterials/gutenberg/data/text/PG10_text.txt"
file3 = open(filename)
filename4 = "/Volumes/COURSES/cs099-00-su19/CourseMaterials/gutenberg/data/text/PG29765_text.txt"
file4 = open(filename)
txt = file.read() + file2.read() + file3.read() + file4.read()
token = txt.split()

sentence = find_sentence(token)
tagged = nltk.pos_tag(sentence)

sentence_struct = []
for (a, b) in tagged:
    sentence_struct.append(b)

# print(sentence_struct)
twog = load_bigrams(txt)
words = random.choice(twog)
word = words[0]
output = word

for i in range(0, len(sentence_struct)):
    print(sentence_struct[i])
    random.shuffle(twog)
    word = new_word(word, twog, sentence_struct[i])
    # print(word)
    output += " " + word

print(output.capitalize() + ".")









# text.generate()
#
# filenametxt = "/Volumes/COURSES/cs099-00-su19/CourseMaterials/gutenberg/data/text/PG41790_text.txt"
# filetxt = open(filenametxt)
# txt = filetxt.read()
# token = list(txt.split(" "))
#
# sentence = find_sentence(token)
# sent = ""
# newsent = ""
# for i in sentence:
#     sent += i + " "
#
#
# text = nltk.Text(word.lower() for word in token)
# print(text.generate())









# pos_tag
#
# def is_punct(letter):
#     return letter == "." or letter == "!" or letter == "?" or letter == "..." or letter == '"'
#
# def find_sentence(token):
#     start = random.randrange(0,len(token))
#     count = 0
#     period1 = 0
#     for i in range (start, len(token)):
#         if len(token[i]) >= 1:
#             if is_punct(token[i][-1]):
#                 count += 1
#                 if count == 1:
#                     period1 = i
#             if count == 2:
#                 return token[period1+1: i+1]
#
# tagged = nltk.pos_tag(sentence)
#
# sentence_struct = []
# for (a, b) in tagged:
#     sentence_struct.append(b)
#
# print(sent)
# print()
# print()
#
# filename = "/Volumes/COURSES/cs099-00-su19/CourseMaterials/gutenberg/data/tokens/PG41790_tokens.txt"
# file = open(filename)
# tokens = file.read().split()
# all_tagged = nltk.pos_tag(tokens)
#
# new_sent = ""
# for j in range(0, len(sentence_struct)):
#     tag = sentence_struct[j]
#     if j % 2 == 1 and j + 1 < len(sentence_struct):
#         new_sent += tagged[j+1][0] + " "
#         continue
#     random.shuffle(all_tagged)
#     for i in range(0, len(all_tagged)):
#         if all_tagged[i][1] == tag:
#             new_sent += all_tagged[i][0] + " "
#             break
#
#
# print(new_sent.capitalize())












# trigrams

# def load_bigrams(text_str):
#     words = text_str.split()
#     bigrams = list(nltk.ngrams(words, 3))
#     return bigrams
#
# def new_word(word, word2, list):
#     for (a, b, c) in list:
#         if word == a and word2 == b:
#             return (b, c)
#
#
#
#
#
#
#
# filename = "/Volumes/COURSES/cs099-00-su19/CourseMaterials/gutenberg/data/text/PG41790_text.txt"
# file = open(filename)
# txt = file.read()
#
# twog = load_bigrams(txt)
# words = random.choice(twog)
# word = words[0]
# word2 = words[1]
# output = word + " " + word2
#
# for i in range(0, 19):
#     random.shuffle(twog)
#     word, word2 = new_word(word, word2, twog)
#     output += " " + word2
#
# print(output.capitalize() + ".")
