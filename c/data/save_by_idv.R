

library(ggplot2)

source("multiplot.R")

setwd("C:/Users/rnc4/git/TEsim/C/data")

# the indexes for selected process variables
source("common.R")

plot_titles<-data.frame(n=names(base.df[xmeas_idx]),l=xmeas_labels)

idv_fname<-"nochan/allidv_plant.dat"
out_dir<-"nochan"

# load the input data file
idv.df <- read.table(idv_fname, header = TRUE, sep = "\t", fill = TRUE, check.names = TRUE)
idv.df$IDV = as.factor(idv.df$IDV)

myf <- function(x, df, od)
{
  y<-df[df$IDV==x,]
  if(x>0)
  {
    ofile<-paste0(od,"/","nochan_idv_",log2(x)+1,".dat")
  }
  else{
    ofile<-paste0(od,"/","nochan_idv_",0,".dat")
  }
  print(ofile)
  write.table(y, file=ofile, append=F, sep = "\t", row.names = F )
  ofile
}

lapply(X=unique(idv.df$IDV), FUN=myf, df=idv.df, od=out_dir)

