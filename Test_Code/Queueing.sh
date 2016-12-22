#!/bin/bash
#
#SBATCH --job-name=Nahrungsnetze #name of the job
#SBATCH --mail-type=END      # notifications for job done
#SBATCH --mail-user=jones@fkp.tu-darmstadt.de  #send-to address
#SBATCH --mem-per-cpu=1000 # memory requirements for the job in MB per core

python Test.py