; Copyright (C) 2018 Brent Bessemer
; All rights reserved.

%struct.px_box = type { float, float, float, float,
    %struct.px_rotation, %struct.px_color, i32, i32 }
%struct.px_rotation = type { float, float, float, float }
%struct.px_color = type { float, float, float, float }

define void @pxZeroBox (i440* %box) {
    store i440 0, i440* %box, align 4
    %real_box = bitcast i440* %box to %struct.px_box*
    %cos = getelementptr inbounds %struct.px_box, %struct.px_box* %real_box,
        i64 0, i32 4, i32 0
    store float 1.0e+0, float* %cos, align 4
    ret void
}

define void @pxSetBoxDims (<4 x float>* %box, float %x, float %y, float %w, float %h) {
    %vec0 = insertelement <4 x float> undef, float %x, i32 0
    %vec1 = insertelement <4 x float> %vec0, float %y, i32 1
    %vec2 = insertelement <4 x float> %vec1, float %w, i32 2
    %vec3 = insertelement <4 x float> %vec2, float %h, i32 3
    store <4 x float> %vec3, <4 x float>* %box, align 4
    ret void
}

define void @pxSetBoxPos (<2 x float>* %box, float %x, float %y) {
    %vec0 = insertelement <2 x float> undef, float %x, i32 0
    %vec1 = insertelement <2 x float> %vec0, float %y, i32 1
    store <2 x float> %vec1, <2 x float>* %box, align 4
    ret void
}

define void @pxSetBoxSize (<2 x float>* %box, float %w, float %h) {
    %vec0 = insertelement <2 x float> undef, float %w, i32 0
    %vec1 = insertelement <2 x float> %vec0, float %h, i32 1
    %dims = getelementptr <2 x float>, <2 x float>* %box, i32 1
    store <2 x float> %vec1, <2 x float>* %dims, align 4
    ret void
}

; Function Attrs: nounwind
declare float @cosf(float) local_unnamed_addr #3

; Function Attrs: nounwind
declare float @sinf(float) local_unnamed_addr #3

;define void @pxSetRotation (<2 x float>* %rot, float %theta) {
;    %cos = call float @cosf(float %theta)
;    %sin = call float @sinf(float %theta)
;    %vec0 = insertelement <2 x float> undef, float %cos, i32 0
;    %vec1 = insertelement <2 x float> %vec0, float %sin, i32 1
;    store <2 x float> %vec1, <2 x float>* %rot, align 4
;    ret void
;}

define void @pxStepRotation (<2 x float>* %rotp, float %dt) {
    %rot = load <2 x float>, <2 x float>* %rotp, align 4
    %dt2 = fmul fast float %dt, %dt
    %cosdt = fsub fast float 1.0, %dt2
    %cosdtv_tmp = insertelement <2 x float> undef, float %cosdt, i32 0
    %cosdtv = insertelement <2 x float> %cosdtv_tmp, float %cosdt, i32 1
    %"-dt" = fsub fast float -0.0, %dt
    %sindtv_tmp = insertelement <2 x float> undef, float %"-dt", i32 0
    %sindtv = insertelement <2 x float> %sindtv_tmp, float %dt, i32 1
    %rotflip = shufflevector <2 x float> %rot, <2 x float> undef,
        <2 x i32> <i32 1, i32 0>
    %rotcos = fmul fast <2 x float> %rot, %cosdtv
    %rotflipsin = fmul fast <2 x float> %rotflip, %sindtv
    %newrot = fadd fast <2 x float> %rotcos, %rotflipsin
    store <2 x float> %newrot, <2 x float>* %rotp, align 4
    ret void
}
