import nltk
from nltk import ngrams

def only50k(stringlist):
    nlist = []

    for i in range(len(stringlist)):
        if (stringlist[i][0].isalpha()):
            nlist.append(stringlist[i].lower())
    return(nlist[:50000])

def get_x_rep (stringlist, rep, size):
    x = list(ngrams(stringlist,size))

    elem = set(x)
    nlist = {}
    for thing in elem:
        z = x.count(thing)
        if (z > rep):
            nlist[thing] = z

    return (nlist)

def discount (shortstringlist, stringlist):

    for elem in shortstringlist.keys():
        for bigelem in stringlist.keys():
            smcheck = ''.join(elem)
            bgcheck = ''.join(bigelem)
            if smcheck in bgcheck:
                if shortstringlist[elem] > stringlist[bigelem]:
                    shortstringlist[elem] -= stringlist[bigelem]
                else:
                    shortstringlist[elem] = 0


def display_legit (dict):
    new = {}
    for elem in dict:
        if dict[elem] > 0:
            new[elem] = dict[elem]
    return(new)

def main():
    fp = (open("elephants_can_remember.txt","r")).read()
    token_list = nltk.word_tokenize(fp)
    token_list = only50k(token_list)

    large = []
    small = []
    final = {}
    small = get_x_rep(token_list, 2, 2)
    for i in range(2,20):
        large = get_x_rep(token_list, 2, i+1)
        discount(small,large)

        final.update(small)
        small = large;
    count = 0
    for elem in final:
        count += final[elem]
    print(count)

if __name__ == '__main__':
    main()
