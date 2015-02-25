!     
! File:   sim_init.f
! Author: rnc4
!
! Created on February 17, 2015, 2:16 PM
!

PRINT*, 'TESIM INITIALIZATION'

! declare model variables

! call the initialization routine
CALL TEINIT(NN,TIME,YY,YP)

! write XMEAS, XMV, and YY to files

! close files

END

