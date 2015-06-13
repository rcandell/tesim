
rm(list=ls())
source("common.R")
source("multiplot.R")
source("calc_kpis_lib.R")

base.df <- read.table("nochan/allidv_plant.dat", header = TRUE, sep = "\t", fill = TRUE, check.names = TRUE)

for (idvs in 2**c(0:14))
{
  print(" ")
  print(" ")
  print(paste0("*******  IDV # ", log2(idvs)," *******"))
  base.time <- base.df[base.df$IDV==idvs,]$time
  base.xmeas <- base.df[base.df$IDV==idvs, xmeas_idx]
  base.xmeas <- base.xmeas[complete.cases(base.xmeas),]
  
  # compares set points with process variable and computes KPI's
  sp_name<-names(base.xmeas)
  sp_value<-c(2800,65,122.9,50,15,22.89,53.8,0.0)
  sp<-data.frame(sp_name,sp_value)
  base.kpis<-kpidf(df = base.xmeas, sp=sp, tm=base.time)
  print(format(base.kpis, digits=2))
  
  # save the kpi's to file
  fname<-paste0("ricker_idv_",log2(idvs),"_kpis.txt")
  write.table(format(base.kpis, digits=2), fname, sep="\t", row.names = F)
}