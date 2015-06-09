install_depends <- function()
{
  install.packages("ggplot2", "reshape2","corrplot","corrgram","grid", "gridExtra")
}

source("multiplot.R")

library(ggplot2)
library(reshape2)
library(lattice)
library(corrplot)
library(grid)
library(gridExtra)

setwd("C:/Users/rnc4/git/TEsim/C/data")

# the indexes for selected process variables
xmeas_idx <- c( 5, c(1:4, 5, 7:12, 14, 15, 17, 23, 25, 40, 42) + 17)
xmv_idx <- c( 5, c(6:17))
all_disturbs <- c( 5, c(60:74, 80))

# load the baseline data
base.df <- read.table("nochan/nochan_noidv_plant.dat", header = TRUE, sep = "\t", fill = TRUE, check.names = TRUE)

base.time <- base.df$time
base.xmeas <- base.df[xmeas_idx]
base.xmv <- base.df[xmv_idx]
base.vars <- base.df[all_idx]

# time plots of all variables
ggplot(base.xmeas, aes_string(x="time", y="Reactor.Pressure.kPa")) +
  geom_point()

ggplot(base.xmeas) +
  geom_point(aes_string(x="time", y="Reactor.Pressure.kPa"))


plotlist<-c()
for (i in names(base.xmeas))
{
  if(i!="time")
  {
    print(i)
    plotlist[[i]]<-ggplot(base.xmeas) +
      geom_line(aes_string(x="time", y=i))  
  }
}
multiplot(plotlist = plotlist, cols=3)

# plot the baseline xmeas data
# par(mfrow = c(4, 4))  # 3 rows and 2 columns
# tdata <- base.df[,xmeas_idx]
# for (name in names(tdata)) {
# #  hist(tdata[,name], breaks=15)
# #  plot(base.time, tdata[,name])
#   plots[[length(plots)+1]] <- qplot(base.time,tdata[,name], xlab = "time.hrs", ylab = name) + geom_line()
# }
# multiplot(plotlist = plots, cols = 4)
# 
# # PCA of the baseline data
# base.vars_cor<-round(cor(base.vars),2)
# #corrplot(base.vars, method="circle")
# corrgram(base.vars, order=TRUE, lower.panel=panel.shade,
#          upper.panel=panel.pie, text.panel=panel.txt,
#          main="Car Milage Data in PC2/PC1 Order") 
# 
# # ITAE for all xmeas
# itae <- function(t, v)
# {
#   
# }
# 
# # PCA
# fit <- prcomp(base.vars,scale = TRUE)
# 
# # plot time series of all xmeas
# 
# 
# # histogram plot for all measured variables
# d <- melt(base.df[,15:55],"time (hrs)")
# ggplot(d,aes(x = value)) + 
#   facet_wrap(~variable,scales = "free_x") + 
#   geom_histogram()
# 
# histogram(base.xmeas$`Reactor Pressure (kPa)`)
# 
# # time series plots for all measured variables
# base.xmeas <- base.df[c(5,18:58)]
# d <- melt(base.df[,15:55],"time (hrs)")
# ggplot(d,aes(x = value)) + 
#   facet_wrap(~variable,scales = "free_x") + 
#   geom_histogram()
# 
# # perform PCA for the base data
# # names(x.base.df)
# pca.name <- "Reactor Pressure (kPa)"
# pca.df<-data.frame(x.base.df["Reactor Pressure (kPa)"])
# x.base.pca <- princomp(mydata, cor=TRUE)
# summary(x.base.pca)
# 
# plant<-read.table("mc_20150531_plant.dat",header = TRUE,sep = "\t",fill = TRUE, check.names = FALSE)
# summary(plant)
# str(plant)
# 
# require(lattice)
# 
# # find unique pq tuples, produce KPI's for each column after pq's
# # could use apply, lapply, etc
# # KPI's will include: mean, std, histogram, ITAE, IAE, Shutdowns, when (from time), why (from SD code)
# # produce a table of the results
# 
# # select unique pq combinations
# pq<-cbind(plant$xmeas_p,plant$xmeas_q,plant$xmv_p,plant$xmv_q)
# colnames(pq)<-c("xmeas_p","xmeas_q","xmv_p","xmv_q")
# pq_u<-unique(pq)
# str(pq)
# str(pq_u)
# summary(pq_u)
# 
# histogram(plant[plant$xmeas_q<0.3|plant$xmeas_p>0.6,]$Reactor.Pressure..kPa.)
# 
# plot(plant[plant$xmeas_q<0.3|plant$xmeas_p>0.6,]$Reactor.Pressure..kPa.)
# 
# 
# # repeat the above for selected IDV's
# # IDV's will include the following:
# # IDV xxx: ????

multiplot <- function(..., plotlist = NULL, file, cols = 1, layout = NULL) {
  require(grid)
  
  plots <- c(list(...), plotlist)
  
  numPlots = length(plots)
  
  if (is.null(layout)) {
    layout <- matrix(seq(1, cols * ceiling(numPlots/cols)),
                     ncol = cols, nrow = ceiling(numPlots/cols))
  }
  
  if (numPlots == 1) {
    print(plots[[1]])
    
  } else {
    grid.newpage()
    pushViewport(viewport(layout = grid.layout(nrow(layout), ncol(layout))))
    
    for (i in 1:numPlots) {
      matchidx <- as.data.frame(which(layout == i, arr.ind = TRUE))
      
      print(plots[[i]], vp = viewport(layout.pos.row = matchidx$row,
                                      layout.pos.col = matchidx$col))
    }
  }
}
