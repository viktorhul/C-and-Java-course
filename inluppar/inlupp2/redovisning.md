# Använda för uppgiften lämpliga abstraktioner
business_logic.c:243, ioopm_replenish

# “Skydda sig” från fel (du skall kunna förklara hur fel hanteras, och motivera valet av mekanism/strategi)
business_logic.c:243, ioopm_replenish
frontend.c:509

# Dokumentera alla publika gränssnittsfunktioner så att en annan programmerare skulle kunna använda implementationen
business_logic.h

# Använda korrekt namngivning (t.ex. ioopm_)
business_logic.h

# Vara indenterat på ett sådant sätt att kontrollflödet är synligt
business_logic.c

# Ha relevanta enhetstester som kan köras automatiskt
tests.c: ioopm_remove_from_cart

# Kunna byggas med make eller liknande verktyg
Ja

# Inte ha minnesläckage eller använda oinitierat minne
Ja

# Inte krascha vid körning med valid indata
Testa gränssnittet

# Inga ha några globala variabler
Nej

# Inga magiska nummer (t.ex. 128 står överallt i koden)
Nej

# Ingen “machokodning” (t.ex. omotiverade oneliners)
Nej

# Återkommande logik är utbruten och inkapslad i funktioner
Ja

# Namn på filer, variabler, funktioner, etc. skall hjälpa läsaren att förstå deras innebörd, och beteende (t.ex. vilka värden som är rimliga, eller vad man kan förvänta sig från ett funktionsanrop)
Ja
