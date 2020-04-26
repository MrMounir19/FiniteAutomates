# dit bestand zal gerund worden en moet voor de nodige input en output zorgen
# je mag hier aanpassingen doen, maar dezelfde functionaliteit moet ondersteund worden
# als er een bestand niet aangemaakt wordt, krijg je daar ook geen punten voor
#
# in dit bestand wordt er van uit gegaan dat er een executable met de naam indiv staat
# dat volgende parameters ondersteunt:
# <algoritme> <inputfile> <outputfile(s)>:
# - algoritme is een uit ssc, mssc, state (voor state elimination), re (omzetting van RE naar eNFA), tfaeq, tfa, productand, productor
# - inputfile is het inputbestand (let op bij tfaeq zijn dat er 2)
# - outputfile is het outputbestand (bij state elimination geef je ook de dot file van de input)
# het is de bedoeling dat hieronder enkel de lijnen komen van de algoritmes die je ondersteunt
./indiv ssc input-ssc1.json output-ssc1.dot
./indiv ssc input-ssc2.json output-ssc2.dot
./indiv mssc input-mssc1.json output-mssc1.dot
./indiv mssc input-mssc2.json output-mssc2.dot
./indiv tfa input-tfa1.json output-tfa1.dot output-tfa1.html
./indiv tfa input-tfa2.json output-tfa2.dot output-tfa2.html
./indiv tfaeq input-tfaeq1.json input-tfaeq2.json output-tfaeq1-2.html
./indiv tfaeq input-tfaeq3.json input-tfaeq4.json output-tfaeq3-4.html
./indiv productand input-productand1.json input-productand2.json output-productand1-2.dot
./indiv productor input-productor1.json input-productor2.json output-productor1-2.dot
