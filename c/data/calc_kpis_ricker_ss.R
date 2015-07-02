

source("common.R")
source("multiplot.R")
source("calc_kpis_lib.R")

base.df <- read.table("nochan/nochan_noidv_plant.dat", header = TRUE, sep = "\t", fill = TRUE, check.names = TRUE)

base.time <- base.df$time
base.xmeas <- base.df[xmeas_idx]
base.xmv <- base.df[xmv_idx]

# compares set points with process variable and computes KPI's
sp_name<-names(base.xmeas)
sp_value<-c(2800,65,122.9,50,15,22.89,53.8,0.0)
sp<-data.frame(sp_name,sp_value)
base.kpis<-kpidf(df = base.xmeas, sp=sp, tm=base.time)
print(base.kpis)

# save the kpi's to file
write.table(base.kpis, "base_kpis.txt", sep="\t", row.names = F)