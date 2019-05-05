/*
 * Mandelbrot set plotter. Based on LLVM Kaleidoscope code snippets.
 * Ported by: Maxim Surkov, Denis Chernikov
 * May 2019
 */

printdensity(d: Real) do
    if (d > 8) then
        StandardIO.put(' ');
    elsif (d > 6) then
        StandardIO.put('.');
    elsif (d > 4) then
        StandardIO.put('-');
    elsif (d > 2) then
        StandardIO.put('+');
    elsif (d > 1) then
        StandardIO.put('*');
    else
        StandardIO.put('#');
    end
end

mandelconverger
(
    real: Real,
    imag: Real,
    iters: Integer,
    creal: Real,
    cimag: Real
) : Real do
    if (iters > 255 || (real * real + imag * imag > 4)) then
        return iters;
    else
        return mandelconverger(real * real - imag * imag + creal, 2 * real * imag + cimag, iters + 1, creal, cimag);
    end
end

mandelconverge(real: Real, imag: Real) : Real do
    return mandelconverger(real, imag, 0, real, imag);
end

mandelhelp(xmin: Real, xmax: Real, xstep: Real, ymin: Real, ymax: Real, ystep: Real) do
    y: Real is ymin;
    while (y < ymax) loop
        x: Real is xmin;
        while (x < xmax) loop
            printdensity(mandelconverge(x, y));
            x := x + xstep;
        end
        StandardIO.put('\n');
        y := y + ystep;
    end
end

mandel(realstart: Real, imagstart: Real, realmag: Real, imagmag: Real) do
    mandelhelp(realstart, realstart + realmag * 78, realmag, imagstart, imagstart + imagmag * 40, imagmag);
end


mandel(-2.3, -1.3, 0.05, 0.07);
mandel(-2, -1, 0.02, 0.04);
