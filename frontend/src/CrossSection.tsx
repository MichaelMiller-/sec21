import { useState } from "react";
import { Button, Col, Form } from "react-bootstrap";
import { RouteComponentProps } from "react-router-dom";
import { DbCrossSection } from "./DatabaseInterface";
// import axios from 'axios';

export type CrossSectionProps = {
    projectId: string;
}

export function CrossSection(props: CrossSectionProps) {

    const [data, setData] = useState<DbCrossSection>({
        id: 0,
        uuid: "",
        name: "",
        area: 0
    })

    const [validated, setValidated] = useState(false);
    // const [status, setStatus] = useState('');

    const handleSubmit = (event: { currentTarget: any; preventDefault: () => void; stopPropagation: () => void; }) => {
        const form = event.currentTarget;
        if (form.checkValidity() === false) {
            event.preventDefault();
            event.stopPropagation();
        }
        console.log("is validated")
        // setValidated(true);
        event.preventDefault();
        console.log(data);     

        // axios.post('https://reqres.in/api/articles', data)
        //     .then(response => setStatus("sefef"));
    };

    const handleInputChange = (e: { target: { name: any; value: any; }; }) => {
        const { name, value } = e.target;
        setData({ ...data, [name]: value, });
    };

    return (
        <Form noValidate validated={validated} onSubmit={handleSubmit}>
            <Form.Row>
                <Form.Group as={Col} md="4" controlId="validationCustom01">
                    <Form.Label>Name</Form.Label>
                    <Form.Control
                        required
                        type="text"
                        name="name"
                        placeholder="Name"
                        value={data.name}
                        onChange={handleInputChange}
                    />
                    <Form.Control.Feedback>Please provide a valid name.</Form.Control.Feedback>
                </Form.Group>
                <Form.Group as={Col} md="4" controlId="validationCustom02">
                    <Form.Label>Area</Form.Label>
                    <Form.Control
                        required
                        type="text"
                        name="area"
                        placeholder="Area"
                        value={data.area}
                        onChange={handleInputChange}
                    />
                    <Form.Control.Feedback>Please provide a valid area.</Form.Control.Feedback>
                </Form.Group>
            </Form.Row>
            <Button type="submit">Submit form</Button>
        </Form>
    );
}

export interface RouterComponentCrossSectionProps extends RouteComponentProps<CrossSectionProps> { }
