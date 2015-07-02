
rm(list=ls())
source("common.R")
source("multiplot.R")
source("calc_kpis_lib.R")

base.df <- read.table("nochan/allidv_plant.dat", header = TRUE, sep = "\t", fill = TRUE, check.names = TRUE)

base.time <- base.df$time
base.xmeas <- base.df[, xmeas_idx]
base.xmeas <- base.xmeas[complete.cases(base.xmeas),]

# compares set points with process variable and computes KPI's
sp_name<-names(base.xmeas)
sp_value<-c(2800,65,122.9,50,15,22.89,53.8,0.0)
sp<-data.frame(sp_name,sp_value)

idvs <- 2**c(0:14)
kpis <- lapply( idvs, 
                FUN = function(ii) {
                  df <- kpidf(df = base.df[base.df$IDV==ii, xmeas_idx], sp=sp, tm=base.time)
                  df$IDV<-ii
                  df
                } )
print(kpis)
kpis <- do.call("rbind", kpis)

fname<-"ricker_allidvs_kpis.txt"
write.table(format(kpis, digits=2), fname, sep="\t", row.names = F)

kpis[sp_name=="Reactor.Pressure.kPa",]
kpis[sp_name=="Hourly.Cost",]
