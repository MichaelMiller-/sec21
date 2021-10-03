import formatDate from "./formatDate";

it('formats PostgreSql timestamp to a human readable string', async () => {
    const result = formatDate(new Date("2021-10-02T12:58:01.639Z"))
    expect(result === "2021-10-02 12:58");
})
