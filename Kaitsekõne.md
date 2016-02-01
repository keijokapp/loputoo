####Tiitelslaid

Tere, austatud komisjon, head kuulajad.

Olen Keijo Kapp ja kaitsen oma lõputööd teemal "Kõrge terviklusega andmeid talletava andmebaasilahenduse prototüüp"
Juhendajad on Tarmo Teder ja kasutajaorganisatsiooni poolt arendusosakonna arhitekt Deivis Treier

####Teine slaid - SMIT
Lahenduse kasutajaks on Siseministeeriumi Infotehnoloogia- ja Arenduskeskus (SMIT), mis
pakub Siseministeeriumi haldusalas erinevaid IT teenused.

See on suur asutus, üle 250 töötaja ning 6 valdkonda, mis kirjeldavad ka tegevusalad.

Lõputöö sai kirjutatud piirivalve ja rahvastikukorralduse valdkonnas.

SMIT arendab ka infosüsteeme, mille puhul kuulub rakendamisele turvameetmete süsteem ISKE.

####Kolmas slaid - ISKE
ISKE on RIA poolt välja töötatud kolmeastmeline etalonturbe süsteem, mida kasutatakse peamiselt avaliku sektori infosüsteemide turvalisuse tagamiseks.

ISKE järgi jagatakse infoturbe kolmeks osaklassiks:
 * konfidentsiaalsus ehk info kättesaamatus volitamata isikutele;
 * käideldavus ehk infosüsteemi ja/või andmete kättesaadavus;
 * terviklus ehk lühidalt öeldes andmete õigsus ja jälgitavus

Lõputöö käsitleb kolme kõrge tervikluse meetme rakendamist, millest 2 on ISKE meetmed.

Nendeks on ISKE meetmete kataloogi järgi:
 * HT.10 Andmebaasi kirjete krüptoaheldamine;
 * HT.34 Digiallkirja kasutamine
 * ning andmete revisioonide hoidmine, mis tuleneb sellest, et andmebaasis olevaid andmeid kunagi reaalselt ei muudeta ega kustutata, välja arvatud siis, kui nende eluiga on otsas.

####Neljas slaid - Krüptoaheldamine
Eesmärk on seostada andmebaasi tabelite kirjed selliselt, et hiljem oleks seoste kontrollimise kägus muudatused tuvastatavad.

Sidumiseks kasutame räsisid, mis on tuletatud iga rea väljade väärtustest ja sellele eelneva rea räsiväärtusest. Niimoodi tekib ketitaoline nähtus.

Andmebaasitarkvarana kasutame me PostgreSQL'i ning krüptoaheldamine on implementeeritud andmebaasis trigeritega.

Viimaste kirjte turvalisuse tagamiseks peab räsid saatma ka kolmandale osapoolele, vastasel juhul saaks andmebaasile ligipääsev osapool andmeid manipuleerida.

####Viies slaid - Krüptoaheldamise joonis
...

#### Kuues slaid - Digiallkirjastamine
Digiallkirjastamine toimub vastavalt Eesti Vabariigi digiallkirjastamise seadusele. See annab andmetele ka juriidilise tõestusväärtuse.

Organisatsiooni kasutatakse rakendusplatvormina Grails raamistikku, mis on kirjutatud Groovy programmeerimiskeeles, mis baseerub Java'l.

Allkirjastamiseks digidoc4j teeki. Versioon, mida ma kasutasin oli 1.0, kuid lõputöö alguses oli see veel beta arendusjärgus ning sellega oli mitmeid olulisi probleeme.

Näiteks ei töötanud see algselt üldse meie platvormil ning sellel puudus krüptoseadmega allkirjastamise tugi.
Lühidalt, me kasutame
 * BDOC tüüpi konteinereid, mis on tehniliselt ZIP failid;
 * SHA512 räšialgoritmi, sest SHA256 ei töötanud (fuq);
 * Krüptoseadet, nagu näiteks HSM, mis suhtleb teegiga PKCS11 standardi abil;
 * Time Mark tüüpi allkirju, sest Sertifitseerimiskeskus soovitab nii.

####Seitsmes slaid - Digiallkirjastamise joonis
...

####Kaheksas slaid - Andmete revisioonide hoidmine
Probleemi lahendamiseks tuleb kohe teha kaks piirangut ja kaks eeldust:

Piiranguteks on:
 * ajalootabelite kasutamine on välistatud, sest andmed on krüptoaheldatud ning neid es saa tõsta tabelite vahel ringi
 * kirjtete muutmine on välistatud samal põhjusel. Seetõttu ei saa me näiteks märkida kirjeid mitteaktiivseteks.

Eeldused on:
 * et umbes 10 revisiooni on iga andmeobjekti, näiteks dokumendi või avalduse, kohta, tõenäoliselt vähem
 * ning revisioonide nimekirja või siis ajaloo pärimine ei ole tihe tegevus, pigem erakorraline

Olen mõeldnud kahe lahenduse peale

####Üheksas slaid - Rekursiivne viitamine eelmisele revisioonile
Esiteks rekursiivne viitamine eelmisele revisioonile
...

####Kümnes slaid - Revisioonide sidumine unikaalse identifikaatoriga
Teine ja kohati kenam võimalus on revisioonide sidumine unikaalsete identifikaatoritega. Asutuse infosüsteemides kasutatakse selliste identifikaatoritena UUID-e...

####Üheteistkümnes slaid - Tänan kuulamast

[*jään lolli näoga komisjoni esimeest vahtima*]
