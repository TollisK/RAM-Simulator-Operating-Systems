# RAM-Simulator-Operating-Systems

## ΠΕΡΙΒΑΛΛΟΝ

Ο κώδικας υλοποιήθηκε σε περιβάλλον Ubuntu 20.04 μέσω IDE VSCode και δοκιμάστηκε σε
Ubuntu 16.04 μέσω PuTTy.

## ΠΕΡΙΕΧΟΜΕΝΑ

Ο φάκελος περιλαμβάνει τα εξής αρχεία:

- README: Τεκμηρίωση και τεχνικές λειτουργίες.
- makefile: Αρχείο για την αυτόματη μεταγλώττιση των c αρχείων σε εκτελέσιμο.

Και τα 2 αρχεία .c και .h για την υλοποίηση του προγράμματος:

- main.c
- main.h

## ΕΚΤΕΛΕΣΗ

Για την εκτέλεση του προγράμματος καλούμε την εντολή make που χρησιμοποιεί την makefile
για την μεταγλώττιση των αρχείων σε εκτελέσιμο. Στην συνέχεια, τρέχουμε το εκτελέσιμο
αρχείο ./main X Y Z όπου τα ορίσματα είναι οι τρείς πληροφορίες που χρειάζονται για την
δοκιμή του προγράμματος. Συγκεκριμένα, το πρώτο όρισμα Χ είναι ο αλγόριθμος που θέλουμε
να χρησιμοποιήσουμε, “LRU” ή “SC” για τους αλγόριθμους Least Recently Used και Second
Chance αντίστοιχα. Σαν δεύτερο όρισμα Υ είναι ο αριθμός των πλαισίων-frames, δηλαδή το
μέγεθος της κύριας μνήμης και το τρίτο όρισμα Ζ δείχνει το πλήθος των αναφορών που
θέλουμε να διαβάσουμε από κάθε αρχείο trace εναλλάξ μέχρι να τον αριθμό με τις συνολικές
αναφορές θα διαβάσει από τον define MAX _PAGE.

Παράδειγμα: ./main LRU 100 250, εκτελεί τον προσομοιωτή με αλγόριθμο Least Recently Used
και μέγεθος κυρίας μνήμης 100 πλαισίων, διαβάζοντας 250 αναφορές εναλλάξ μέχρι να
διαβαστούν 10000 αναφορές.

Το σύνολο των αναφορών που επιθυμούμε να εξεταστούν προσδιορίζεται στο define
MAX_PAGE με προκαθορισμένη τιμή 10000.

Το εκτελέσιμο πρέπει να βρίσκεται στον ίδιο φάκελο με τα 2 αρχεία trace gcc.trace και
bzip.trace ώστε να διαβαστούν.


## ΠΙΝΑΚΑΣ ΚΑΤΑΚΕΡΜΑΤΙΣΜΟΥ

Για την υλοποίηση του προγράμματος χρησιμοποιούμε έναν πίνακα κατακερματισμού με
κλειδί το id της σελίδας και τιμή την θέση του πλαισίου που του αντιστοιχεί. Συγκεκριμένα,
χρειάζεται μια συνάρτηση κατακερματισμού και έναν πίνακα από λίστες όπου η συνάρτηση θα
επιστρέφει την θέση που κατακερματίζεται το id στον πίνακα και ο πίνακας θα αποθηκεύει
στις λίστες το id της σελίδας και την θέση του πλαισίου. Ο πίνακας έχει μέγεθος όσο τον
αριθμό των πλαισίων, αυτή η υλοποίηση είναι απαραίτητη καθώς πολλά id σελίδας
κατακερματίζονται στην ίδια θέση του πίνακα.

Η λίστα αποτελείται από συνδεδεμένους κόμβους hash_block με τα εξής στοιχεία:

- int framenum: Το id της σελίδας που χρησιμοποιείται ως κλειδί πλαίσιο για την λίστα.
- int pagenum: Η θέση που αποθηκεύεται η σελίδα στην κύρια μνήμη.
- Hash_block *next: Δείκτης στον επόμενο κόμβο για την υλοποίηση λίστας.

Επίσης χρησιμοποιούνται οι συνηθισμένες συναρτήσεις διαχείρισης της λίστας:

- void push(hash_block ** head, int frame, int page): Προσθέτει στην λίστα τις τιμές
    frame και page.
- delete(hash_block**head,int page): Διαγράφει από την λίστα τον κόμβο με id την τιμή
    του page.
- bool search_list(hash_block *, int page):Επιστρέφει True αν υπάρχει κόμβος στην λίστα
    με κλειδί το page.
- void deleteList(struct node** head_ref): Διαγραφή όλης της λίστας.

## ΚΥΡΙΑ ΜΝΗΜΗ-RAM

Η προσομοίωση της κύριας μνήμης υλοποιείται με έναν πίνακα από πλαίσια-frames με
μέγεθος την τιμή Y των ορισμάτων, όπου ένα πλαίσιο είναι ένα struct με δεδομένα:

- unsigned long long time: Χρόνος από την αρχή της εκτέλεσης του προγράμματος και
    μετριέται σε millisecond.
- int in_disc: Δυαδική τιμή που δείχνει αν βρίσκεται στην κύρια μνήμη.
- int write: Δυαδική τιμή που είναι 1 όταν ο δείκτης στην αναφορά είναι W αλλιώς 0.
- Int full: Δυαδική τιμή που ισούται με 1 όταν έχει αποθηκευτεί σελίδα, αλλιώς 0.
- Int page: id της σελίδας που αποθηκεύτηκε.
- int ref_bit: Δυαδική τιμή που χρησιμοποιείται από τον αλγόριθμο Second Chance και
    ισούται με 1 όταν κατά την εισαγωγή νέας αναφοράς υπάρχει ήδη στην κύρια μνήμη.


## ΓΕΝΙΚΗ ΥΛΟΠΟΙΗΣΗ ΠΡΟΣΟΜΟΙΩΤΗ

Το πρόγραμμα αποθηκεύει τα ορίσματα της main στις κατάλληλες μεταβλητές και αρχικοποιεί
την κύρια μνήμη-RAM και τον πίνακα κατακερματισμού. Στην συνέχεια, διαβάζει από κάθε
αρχείο trace (πρώτα το bzip.trace και ύστερα το gcc.trace) όσες αναφορές έχει δεχτεί ως
όρισμα εναλλάξ μέχρι να φτάσει τον μέγιστο αριθμό αναφορών που προσδιορίζεται στο
define. Κατά την εισαγωγή μιας αναφοράς στον προσομοιωτή αποθηκεύεται ο δείκτης W ή R
και η ταυτοποίηση-id της σελίδας που τον μετατρέπει από δεκαεξαδικό σε δεκαδικό μέσω της
συνάρτησης hextodec. Για την αποθήκευση της σελίδας σε κάποιο frame πρέπει πρώτα να
εξετάσουμε την πιθανότητα η σελίδα αυτή να υπάρχει ήδη στην κύρια μνήμη, εδώ λαμβάνει
μέρος ο πίνακας κατακερματισμού. Οπότε, χρησιμοποιούμε την συνάρτηση κατακερματισμού
για τιμή το id της σελίδας και επιστρέφει σε ποια λίστα κατακερματίζεται, ύστερα εξετάζουμε
αν το κλειδί αυτό βρίσκεται στην λίστα. Αν υπάρχει στον πίνακα, οπότε και στην κύρια μνήμη,
ενημερώνουμε τον χρόνο μέσω της συνάρτησης current_timestamp η οποία μετράει σε
millisecond και κάνει το reference bit του πλαισίου 1 για την χρήση του στον αλγόριθμο
Second Chance. Αν όμως δεν υπάρχει στον πίνακα πρέπει να δούμε αν η κύρια μνήμη είναι
γεμάτη. Στην περίπτωση που δεν είναι γεμάτη, βρίσκει γραμμικά μια κενή θέση και εκεί
αρχικοποιεί τα δεδομένα της και ταυτόχρονα εισάγει και το id της σελίδας με την θέση του
πλαισίου στον πίνακα κατακερματισμού. Ενώ, αν η κύρια μνήμη είναι γεμάτη τότε εκτελεί
έναν από τους αλγόριθμους (ανάλογα το όρισμα της main) LRU ή SC που επιστρέφουν την
θέση του πλαισίου που πρέπει να μηδενιστεί ώστε να αποθηκευτεί η καινούργια σελίδα. Στην
συνέχεια, βρίσκει σε ποιο πλαίσιο ήταν αποθηκευμένη αυτή η σελίδα και ενημερώνει τα
δεδομένα με εκείνα της νέας σελίδας, εισάγοντας και τις 2 τιμές id και θέση τον πίνακα
κατακερματισμού. Τέλος, διαγράφει τον κόμβο με κλειδί το id της σελίδας από τον πίνακα.

## ΑΛΓΟΡΙΘΜΟΙ ΑΝΤΙΚΑΤΑΣΤΑΣΗΣ

Για την εύρεση της σελίδας που πρέπει να αφαιρέσουμε από την κύρια μνήμη για την
εισαγωγή καινούργιας υλοποιούνται 2 αλγόριθμοι LRU και SC.

- Least Recently Used: Δέχεται ως όρισμα την μνήμη RAM και το μέγεθος της και την
    διατρέχει βρίσκοντας το πλαίσιο με τον μικρότερο χρόνο, δηλαδή αυτή που εισάχθηκε
    πρώτη. Επιστρέφει την θέση του πλαισίου που βρέθηκε ο χρόνος αυτός.
- Second Chance: Είναι παρόμοια με την LRU με την διαφορά ότι αν το πλαίσιο έχει
    reference bit 1 τότε δεν θα το εξετάσει. Το ref_bit γίνεται 1 όταν κατά την εισαγωγή
    μιας αναφοράς στην μνήμη υπάρχει ήδη η σελίδα αυτή. Στο τέλος μηδενίζει στα
    πλαίσια που κανονικά θα έπρεπε να επιστραφεί το ref_bit τους.


