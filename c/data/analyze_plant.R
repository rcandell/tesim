
setwd("C:/Users/rnc4/git/TEsim/C/data")
x.base.df <- read.table("nochan_plant.dat", header = TRUE, sep = "\t", fill = TRUE, check.names = FALSE)

# perform PCA for the base data
# names(x.base.df)
pca.name <- "Reactor Pressure (kPa)"
pca.df<-data.frame(x.base.df["Reactor Pressure (kPa)"])
x.base.pca <- princomp(mydata, cor=TRUE)
summary(x.base.pca)

plant<-read.table("mc_20150531_plant.dat",header = TRUE,sep = "\t",fill = TRUE, check.names = FALSE)
summary(plant)
str(plant)

require(lattice)

# find unique pq tuples, produce KPI's for each column after pq's
# could use apply, lapply, etc
# KPI's will include: mean, std, histogram, ITAE, IAE, Shutdowns, when (from time), why (from SD code)
# produce a table of the results

# select unique pq combinations
pq<-cbind(plant$xmeas_p,plant$xmeas_q,plant$xmv_p,plant$xmv_q)
colnames(pq)<-c("xmeas_p","xmeas_q","xmv_p","xmv_q")
pq_u<-unique(pq)
str(pq)
str(pq_u)
summary(pq_u)

histogram(plant[plant$xmeas_q<0.3|plant$xmeas_p>0.6,]$Reactor.Pressure..kPa.)

plot(plant[plant$xmeas_q<0.3|plant$xmeas_p>0.6,]$Reactor.Pressure..kPa.)


# repeat the above for selected IDV's
# IDV's will include the following:
# IDV xxx: ????


